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
 mov qword ptr [r12 + 2336], rax
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
 .string "node(val,lson,rson)"
 xchain0_n1_α:
# BOX CALL data(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn2: .string "data"
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
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n7_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "x"
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n7_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "y"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n7_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "z"
 xchain0_n5_α:
# BOX CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+272] -> [zr+208]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [zr+304] -> [zr+224]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 232], rax
# marshal arg2 = producer-box slot [zr+336] -> [zr+240]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lbynamefn6: .string "node"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 208]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je xchain0_n7_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_ASSIGN gva
 xchain0_n6_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n7_α
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
 xchain0_n8_α:
# BOX CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+560] -> [zr+528]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn9: .string "datatype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n12_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n10_α:
 mov rdi, qword ptr [rip + .Lx11_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n9_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "lcase"
 xchain0_n11_α:
# BOX CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+816] -> [zr+784]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 792], rax
  .section .rodata
  .Lbynamefn12: .string "val"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 784]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp xchain0_n12_α
 xchain0_n12_α:
# BOX CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn13: .string "node"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 960]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n16_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n13_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n9_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "ucase"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 848], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n12_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "x"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n16_α
# IR_VAR
 xchain0_n16_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n20_α
 xchain0_n17_α:
# BOX CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+448]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 456], rax
# marshal arg1 = producer-box slot [zr+592] -> [zr+464]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 472], rax
# marshal arg2 = producer-box slot [zr+608] -> [zr+480]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn18: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 448]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n21_α
 xchain0_n17_β:
 jmp xchain0_n9_α
 xchain0_n18_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+720]
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 728], rax
# marshal arg1 = producer-box slot [zr+848] -> [zr+736]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 736], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 744], rax
  .section .rodata
  .Lbynamefn19: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n22_α
 xchain0_n18_β:
 jmp xchain0_n12_α
 xchain0_n19_α:
# BOX CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1072] -> [zr+1040]
 mov rax, qword ptr [r12 + 1072]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 1080]
 mov qword ptr [r12 + 1048], rax
  .section .rodata
  .Lbynamefn20: .string "rson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 1040]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n23_α
 xchain0_n19_β:
 jmp xchain0_n20_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 1152], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n24_α
 xchain0_n20_β:
 jmp xchain0_n29_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 624], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp xchain0_n9_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "NODE"
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n26_α
 xchain0_n22_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "FAIL 1115/002: field accessor val"
 xchain0_n23_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1024] -> [zr+992]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 1000], rax
  .section .rodata
  .Lbynamefn24: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 992]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n27_α
 xchain0_n23_β:
 jmp xchain0_n20_α
 xchain0_n24_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1152] -> [zr+1200]
 mov rax, qword ptr [r12 + 1152]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1160]
 mov qword ptr [r12 + 1208], rax
  .section .rodata
  .Lrkfn26: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn26]
 lea rsi, [r12 + 1200]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp xchain0_n29_α
 xchain0_n25_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+384]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 392], rax
# marshal arg1 = producer-box slot [zr+624] -> [zr+400]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 896]
 mov rdx, qword ptr [r12 + 904]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n31_α
 xchain0_n27_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 1115/003: unset field is null"
# IR_VAR
 xchain0_n28_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp xchain0_n32_α
 xchain0_n28_β:
 jmp xchain0_n29_α
# IR_VAR
 xchain0_n29_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 jmp xchain0_n33_α
 xchain0_n29_β:
 jmp xchain0_n34_α
# IR_LIT_STRING
 xchain0_n30_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n35_α
 xchain0_n30_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "FAIL 1115/001: datatype of node"
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [r12 + 1120]
 mov rdx, qword ptr [r12 + 1128]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp main_γ
 xchain0_n31_β:
 jmp main_γ
 xchain0_n32_α:
# BOX CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1280] -> [zr+1248]
 mov rax, qword ptr [r12 + 1280]
 mov qword ptr [r12 + 1248], rax
 mov rax, qword ptr [r12 + 1288]
 mov qword ptr [r12 + 1256], rax
  .section .rodata
  .Lbynamefn33: .string "lson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn33]
 lea rsi, [r12 + 1248]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n36_α
 xchain0_n32_β:
 jmp xchain0_n29_α
 xchain0_n33_α:
# BOX CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1520] -> [zr+1488]
 mov rax, qword ptr [r12 + 1520]
 mov qword ptr [r12 + 1488], rax
 mov rax, qword ptr [r12 + 1528]
 mov qword ptr [r12 + 1496], rax
  .section .rodata
  .Lbynamefn34: .string "lson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 1488]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n37_α
 xchain0_n33_β:
 jmp xchain0_n34_α
# IR_LIT_STRING
 xchain0_n34_α:
 mov qword ptr [r12 + 1744], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 1752], rax
 jmp xchain0_n38_α
 xchain0_n34_β:
 jmp xchain0_n42_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "b"
# IR_ASSIGN gva
 xchain0_n35_α:
 mov rax, qword ptr [r12 + 672]
 mov rdx, qword ptr [r12 + 680]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp main_γ
 xchain0_n35_β:
 jmp main_γ
# IR_VAR
 xchain0_n36_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp xchain0_n39_α
 xchain0_n36_β:
 jmp xchain0_n29_α
 xchain0_n37_α:
# BOX CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1472] -> [zr+1440]
 mov rax, qword ptr [r12 + 1472]
 mov qword ptr [r12 + 1440], rax
 mov rax, qword ptr [r12 + 1480]
 mov qword ptr [r12 + 1448], rax
  .section .rodata
  .Lbynamefn38: .string "rson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn38]
 lea rsi, [r12 + 1440]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1424], rax
 mov qword ptr [r12 + 1432], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n40_α
 xchain0_n37_β:
 jmp xchain0_n34_α
 xchain0_n38_α:
# BOX CALL value(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1744] -> [zr+1712]
 mov rax, qword ptr [r12 + 1744]
 mov qword ptr [r12 + 1712], rax
 mov rax, qword ptr [r12 + 1752]
 mov qword ptr [r12 + 1720], rax
  .section .rodata
  .Lbynamefn39: .string "value"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn39]
 lea rsi, [r12 + 1712]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1696], rax
 mov qword ptr [r12 + 1704], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n41_α
 xchain0_n38_β:
 jmp xchain0_n42_α
# IR_ASSIGN_VAR
 xchain0_n39_α:
 mov rdi, qword ptr [r12 + 1232]
 mov rsi, qword ptr [r12 + 1240]
 mov rdx, qword ptr [r12 + 1312]
 mov rcx, qword ptr [r12 + 1320]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp xchain0_n29_α
 xchain0_n39_β:
 jmp xchain0_n29_α
# IR_LIT_STRING
 xchain0_n40_α:
 mov qword ptr [r12 + 1552], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 1560], rax
 jmp xchain0_n43_α
 xchain0_n40_β:
 jmp xchain0_n34_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "z"
# IR_VAR
 xchain0_n41_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1776], rax
 mov qword ptr [r12 + 1784], rdx
 jmp xchain0_n44_α
 xchain0_n41_β:
 jmp xchain0_n42_α
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [r12 + 1856], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 1864], rax
 jmp xchain0_n45_α
 xchain0_n42_β:
 jmp xchain0_n49_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string ""
 xchain0_n43_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1424] -> [zr+1376]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1376], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1384], rax
# marshal arg1 = producer-box slot [zr+1552] -> [zr+1392]
 mov rax, qword ptr [r12 + 1552]
 mov qword ptr [r12 + 1392], rax
 mov rax, qword ptr [r12 + 1560]
 mov qword ptr [r12 + 1400], rax
  .section .rodata
  .Lbynamefn44: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn44]
 lea rsi, [r12 + 1376]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n46_α
 xchain0_n43_β:
 jmp xchain0_n34_α
 xchain0_n44_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1696] -> [zr+1648]
 mov rax, qword ptr [r12 + 1696]
 mov qword ptr [r12 + 1648], rax
 mov rax, qword ptr [r12 + 1704]
 mov qword ptr [r12 + 1656], rax
# marshal arg1 = producer-box slot [zr+1776] -> [zr+1664]
 mov rax, qword ptr [r12 + 1776]
 mov qword ptr [r12 + 1664], rax
 mov rax, qword ptr [r12 + 1784]
 mov qword ptr [r12 + 1672], rax
  .section .rodata
  .Lbynamefn45: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn45]
 lea rsi, [r12 + 1648]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 cmp eax, 99
 je xchain0_n42_α
 jmp xchain0_n47_α
 xchain0_n44_β:
 jmp xchain0_n42_α
 xchain0_n45_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1856] -> [zr+1904]
 mov rax, qword ptr [r12 + 1856]
 mov qword ptr [r12 + 1904], rax
 mov rax, qword ptr [r12 + 1864]
 mov qword ptr [r12 + 1912], rax
  .section .rodata
  .Lrkfn48: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn48]
 lea rsi, [r12 + 1904]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1888], rax
 mov qword ptr [r12 + 1896], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n48_α
 xchain0_n45_β:
 jmp xchain0_n49_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 1600], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 1608], rax
 jmp xchain0_n50_α
 xchain0_n46_β:
 jmp main_γ
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "FAIL 1115/004: nested accessor after mutate"
# IR_LIT_STRING
 xchain0_n47_α:
 mov qword ptr [r12 + 1824], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 1832], rax
 jmp xchain0_n51_α
 xchain0_n47_β:
 jmp main_γ
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 1115/005: value() by variable name"
# IR_VAR
 xchain0_n48_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1984], rax
 mov qword ptr [r12 + 1992], rdx
 jmp xchain0_n52_α
 xchain0_n48_β:
 jmp xchain0_n49_α
# IR_VAR
 xchain0_n49_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 2176], rax
 mov qword ptr [r12 + 2184], rdx
 jmp xchain0_n53_α
 xchain0_n49_β:
 jmp xchain0_n54_α
# IR_ASSIGN gva
 xchain0_n50_α:
 mov rax, qword ptr [r12 + 1600]
 mov rdx, qword ptr [r12 + 1608]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 jmp main_γ
 xchain0_n50_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [r12 + 1824]
 mov rdx, qword ptr [r12 + 1832]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1808], rax
 mov qword ptr [r12 + 1816], rdx
 jmp main_γ
 xchain0_n51_β:
 jmp main_γ
 xchain0_n52_α:
# BOX CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1984] -> [zr+1952]
 mov rax, qword ptr [r12 + 1984]
 mov qword ptr [r12 + 1952], rax
 mov rax, qword ptr [r12 + 1992]
 mov qword ptr [r12 + 1960], rax
  .section .rodata
  .Lbynamefn53: .string "val"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [r12 + 1952]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1936], rax
 mov qword ptr [r12 + 1944], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n55_α
 xchain0_n52_β:
 jmp xchain0_n49_α
 xchain0_n53_α:
# BOX CALL val(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2176] -> [zr+2144]
 mov rax, qword ptr [r12 + 2176]
 mov qword ptr [r12 + 2144], rax
 mov rax, qword ptr [r12 + 2184]
 mov qword ptr [r12 + 2152], rax
  .section .rodata
  .Lbynamefn54: .string "val"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn54]
 lea rsi, [r12 + 2144]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2128], rax
 mov qword ptr [r12 + 2136], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n56_α
 xchain0_n53_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n54_α:
 mov qword ptr [r12 + 2304], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 2312], rax
 jmp xchain0_n57_α
 xchain0_n54_β:
 jmp main_γ
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "PASS 1115_data_basic (6/6)"
# IR_LIT_STRING
 xchain0_n55_α:
 mov qword ptr [r12 + 2016], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 2024], rax
 jmp xchain0_n58_α
 xchain0_n55_β:
 jmp xchain0_n49_α
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "new"
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [r12 + 2208], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 2216], rax
 jmp xchain0_n59_α
 xchain0_n56_β:
 jmp xchain0_n54_α
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "new"
# IR_ASSIGN gva
 xchain0_n57_α:
 mov rax, qword ptr [r12 + 2304]
 mov rdx, qword ptr [r12 + 2312]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2288], rax
 mov qword ptr [r12 + 2296], rdx
 jmp main_γ
 xchain0_n57_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n58_α:
 mov rdi, qword ptr [r12 + 1936]
 mov rsi, qword ptr [r12 + 1944]
 mov rdx, qword ptr [r12 + 2016]
 mov rcx, qword ptr [r12 + 2024]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [r12 + 2048], rax
 mov qword ptr [r12 + 2056], rdx
 jmp xchain0_n49_α
 xchain0_n58_β:
 jmp xchain0_n49_α
 xchain0_n59_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2128] -> [zr+2080]
 mov rax, qword ptr [r12 + 2128]
 mov qword ptr [r12 + 2080], rax
 mov rax, qword ptr [r12 + 2136]
 mov qword ptr [r12 + 2088], rax
# marshal arg1 = producer-box slot [zr+2208] -> [zr+2096]
 mov rax, qword ptr [r12 + 2208]
 mov qword ptr [r12 + 2096], rax
 mov rax, qword ptr [r12 + 2216]
 mov qword ptr [r12 + 2104], rax
  .section .rodata
  .Lbynamefn60: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn60]
 lea rsi, [r12 + 2080]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2064], rax
 mov qword ptr [r12 + 2072], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 2256], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 2264], rax
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp main_γ
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string "FAIL 1115/006: mutate field and read back"
# IR_ASSIGN gva
 xchain0_n61_α:
 mov rax, qword ptr [r12 + 2256]
 mov rdx, qword ptr [r12 + 2264]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2240], rax
 mov qword ptr [r12 + 2248], rdx
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
 mov rdi, qword ptr [r12 + 2336]
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

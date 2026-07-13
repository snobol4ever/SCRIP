  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "b"
  .Lgvan2: .string "output"
  .Lgvan3: .string "c"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
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
  mov edx, 4
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
  mov qword ptr [r12 + 1624], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1616], rax
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
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "clunk(value,lson)"
 xchain0_n3_α:
# BOX CALL data(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+224] -> [zr+192]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn4: .string "data"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 192]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n9_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "x"
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n9_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "y"
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "z"
 xchain0_n7_α:
# BOX CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+352] -> [zr+288]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 296], rax
# marshal arg1 = producer-box slot [zr+384] -> [zr+304]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 312], rax
# marshal arg2 = producer-box slot [zr+416] -> [zr+320]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn8: .string "node"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 288]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n8_α:
 mov rax, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n9_α
 xchain0_n9_α:
# BOX CALL node(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn10: .string "node"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 480]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n14_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string ""
 xchain0_n12_α:
# BOX IR_CALL SNO$WANTNM(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+496] -> [zr+544]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lrkfn14: .string "SNO$WANTNM"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn14]
 lea rsi, [r12 + 544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n13_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n14_α
# IR_VAR
 xchain0_n14_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n17_α
 xchain0_n15_α:
# BOX CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+624] -> [zr+592]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 600], rax
  .section .rodata
  .Lbynamefn16: .string "lson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 592]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp xchain0_n14_α
 xchain0_n16_α:
# BOX CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+864] -> [zr+832]
 mov rax, qword ptr [r12 + 864]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 872]
 mov qword ptr [r12 + 840], rax
  .section .rodata
  .Lbynamefn17: .string "lson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 832]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 1088], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp xchain0_n24_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "b"
# IR_VAR
 xchain0_n18_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp xchain0_n14_α
 xchain0_n19_α:
# BOX CALL rson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+816] -> [zr+784]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 792], rax
  .section .rodata
  .Lbynamefn20: .string "rson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 784]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n17_α
 xchain0_n20_α:
# BOX CALL value(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1088] -> [zr+1056]
 mov rax, qword ptr [r12 + 1088]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 1096]
 mov qword ptr [r12 + 1064], rax
  .section .rodata
  .Lbynamefn21: .string "value"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 1056]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n24_α
# IR_ASSIGN_VAR
 xchain0_n21_α:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 mov rdx, qword ptr [r12 + 656]
 mov rcx, qword ptr [r12 + 664]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n14_α
 xchain0_n21_β:
 jmp xchain0_n14_α
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n25_α
 xchain0_n22_β:
 jmp xchain0_n17_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "z"
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 jmp xchain0_n26_α
 xchain0_n23_β:
 jmp xchain0_n24_α
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 1280], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n27_α
 xchain0_n24_β:
 jmp xchain0_n34_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "alpha"
 xchain0_n25_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+720]
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 728], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+736]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 736], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 744], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n28_α
 xchain0_n25_β:
 jmp xchain0_n17_α
 xchain0_n26_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1040] -> [zr+992]
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 1000], rax
# marshal arg1 = producer-box slot [zr+1120] -> [zr+1008]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1016], rax
  .section .rodata
  .Lbynamefn27: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n29_α
 xchain0_n26_β:
 jmp xchain0_n24_α
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [r12 + 1312], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 1320], rax
 jmp xchain0_n30_α
 xchain0_n27_β:
 jmp xchain0_n34_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "beta"
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 944], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n31_α
 xchain0_n28_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 1116/001: node.rson after clunk data def"
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [r12 + 1168], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n32_α
 xchain0_n29_β:
 jmp main_γ
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "FAIL 1116/002: value() still works after clunk"
 xchain0_n30_α:
# BOX CALL clunk(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1280] -> [zr+1232]
 mov rax, qword ptr [r12 + 1280]
 mov qword ptr [r12 + 1232], rax
 mov rax, qword ptr [r12 + 1288]
 mov qword ptr [r12 + 1240], rax
# marshal arg1 = producer-box slot [zr+1312] -> [zr+1248]
 mov rax, qword ptr [r12 + 1312]
 mov qword ptr [r12 + 1248], rax
 mov rax, qword ptr [r12 + 1320]
 mov qword ptr [r12 + 1256], rax
  .section .rodata
  .Lbynamefn31: .string "clunk"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn31]
 lea rsi, [r12 + 1232]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 cmp eax, 99
 je xchain0_n34_α
 jmp xchain0_n33_α
 xchain0_n30_β:
 jmp xchain0_n34_α
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp main_γ
 xchain0_n31_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n32_α:
 mov rax, qword ptr [r12 + 1168]
 mov rdx, qword ptr [r12 + 1176]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp main_γ
 xchain0_n32_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n33_α:
 mov rax, qword ptr [r12 + 1216]
 mov rdx, qword ptr [r12 + 1224]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n34_α
# IR_VAR
 xchain0_n34_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n36_α
 xchain0_n35_α:
# BOX CALL lson(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1456] -> [zr+1424]
 mov rax, qword ptr [r12 + 1456]
 mov qword ptr [r12 + 1424], rax
 mov rax, qword ptr [r12 + 1464]
 mov qword ptr [r12 + 1432], rax
  .section .rodata
  .Lbynamefn36: .string "lson"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn36]
 lea rsi, [r12 + 1424]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1408], rax
 mov qword ptr [r12 + 1416], rdx
 cmp eax, 99
 je xchain0_n36_α
 jmp xchain0_n37_α
 xchain0_n35_β:
 jmp xchain0_n36_α
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [r12 + 1584], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 1592], rax
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "PASS 1116_data_overlap (3/3)"
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [r12 + 1488], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 1496], rax
 jmp xchain0_n39_α
 xchain0_n37_β:
 jmp xchain0_n36_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "beta"
# IR_ASSIGN gva
 xchain0_n38_α:
 mov rax, qword ptr [r12 + 1584]
 mov rdx, qword ptr [r12 + 1592]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 jmp main_γ
 xchain0_n38_β:
 jmp main_γ
 xchain0_n39_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1408] -> [zr+1360]
 mov rax, qword ptr [r12 + 1408]
 mov qword ptr [r12 + 1360], rax
 mov rax, qword ptr [r12 + 1416]
 mov qword ptr [r12 + 1368], rax
# marshal arg1 = producer-box slot [zr+1488] -> [zr+1376]
 mov rax, qword ptr [r12 + 1488]
 mov qword ptr [r12 + 1376], rax
 mov rax, qword ptr [r12 + 1496]
 mov qword ptr [r12 + 1384], rax
  .section .rodata
  .Lbynamefn40: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 1360]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 cmp eax, 99
 je xchain0_n36_α
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n36_α
# IR_LIT_STRING
 xchain0_n40_α:
 mov qword ptr [r12 + 1536], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 1544], rax
 jmp xchain0_n41_α
 xchain0_n40_β:
 jmp main_γ
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "FAIL 1116/003: clunk.lson accessor"
# IR_ASSIGN gva
 xchain0_n41_α:
 mov rax, qword ptr [r12 + 1536]
 mov rdx, qword ptr [r12 + 1544]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 jmp main_γ
 xchain0_n41_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 1624]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1616]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 1624]
pop r12
ret

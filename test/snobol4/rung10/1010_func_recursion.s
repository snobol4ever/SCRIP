  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "fact"
  .Lgvan1: .string "n"
  .Lgvan2: .string "output"
  .Lgvan3: .string "facto"
  .Lgvan4: .string "fact2_entry"
  .Lgvan5: .string "fact2"
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
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 6
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
 mov qword ptr [r12 + 1328], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 48], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 56], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "fact(n)"
 xchain0_n1_α:
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
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx4_0:
 .quad 5
 xchain0_n3_α:
# BOX IR_CALL fact(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+400] -> [r12+384]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn6: .string "fact"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 384]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx7_0:
 .quad 120
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n10_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "facto"
 xchain0_n6_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn10: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn10]
 lea rsi, [r12 + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
xchain0_n6_β:
 jmp xchain0_n5_α
 xchain0_n7_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+576] -> [r12+560]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lrkfn12: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn12]
 lea rsi, [r12 + 560]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "FAIL 1010/001: fact(5)=120"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "fact"
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 720], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n16_α
.Lx15_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n11_α:
 mov rax, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
 xchain0_n12_α:
# BOX IR_CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+544] -> [r12+512]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 520], rax
# marshal arg1 = producer-box slot [r12+592] -> [r12+528]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn18: .string "opsyn"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n14_α
xchain0_n12_β:
 jmp xchain0_n10_α
 xchain0_n13_α:
# BOX IR_CALL facto(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+720] -> [r12+704]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 712], rax
  .section .rodata
  .Lbynamefn20: .string "facto"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 704]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
xchain0_n13_β:
 jmp xchain0_n16_α
 xchain0_n14_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+496] -> [r12+480]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn22: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn22]
 lea rsi, [r12 + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n17_α
xchain0_n14_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx23_0:
 .quad 24
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n25_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "fact2(n)"
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 624], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 1010/002: opsyn alias"
 xchain0_n18_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+688] -> [r12+656]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 664], rax
# marshal arg1 = producer-box slot [r12+736] -> [r12+672]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn27: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n21_α
xchain0_n18_β:
 jmp xchain0_n16_α
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n25_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "fact2_entry"
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [r12 + 624]
 mov rdx, qword ptr [r12 + 632]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 1010/003: facto(4)=24 via alias"
 xchain0_n22_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+880] -> [r12+864]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 872], rax
  .section .rodata
  .Lrkfn32: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn32]
 lea rsi, [r12 + 864]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 768]
 mov rdx, qword ptr [r12 + 776]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
 xchain0_n24_α:
# BOX IR_CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+832] -> [r12+800]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 808], rax
# marshal arg1 = producer-box slot [r12+848] -> [r12+816]
 mov rax, qword ptr [r12 + 848]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 856]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn35: .string "define"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 800]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n25_α
xchain0_n24_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [r12 + 1232], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 1240], rax
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n28_α
.Lx36_0:
 .quad 6
 xchain0_n26_α:
# BOX IR_CALL fact2(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1232] -> [r12+1216]
 mov rax, qword ptr [r12 + 1232]
 mov qword ptr [r12 + 1216], rax
 mov rax, qword ptr [r12 + 1240]
 mov qword ptr [r12 + 1224], rax
  .section .rodata
  .Lbynamefn38: .string "fact2"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn38]
 lea rsi, [r12 + 1216]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n27_α
xchain0_n26_β:
 jmp xchain0_n28_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 1248], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n28_α
.Lx39_0:
 .quad 720
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 1312], 1
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 1320], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "PASS 1010_func_recursion (4/4)"
 xchain0_n29_α:
# BOX IR_CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1200] -> [r12+1168]
 mov rax, qword ptr [r12 + 1200]
 mov qword ptr [r12 + 1168], rax
 mov rax, qword ptr [r12 + 1208]
 mov qword ptr [r12 + 1176], rax
# marshal arg1 = producer-box slot [r12+1248] -> [r12+1184]
 mov rax, qword ptr [r12 + 1248]
 mov qword ptr [r12 + 1184], rax
 mov rax, qword ptr [r12 + 1256]
 mov qword ptr [r12 + 1192], rax
  .section .rodata
  .Lbynamefn42: .string "ne"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn42]
 lea rsi, [r12 + 1168]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n31_α
xchain0_n29_β:
 jmp xchain0_n28_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 1312]
 mov rdx, qword ptr [r12 + 1320]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n31_α:
 mov qword ptr [r12 + 1280], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp main_γ
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "FAIL 1010/004: fact2(6)=720 alt entry"
# IR_ASSIGN gva
 xchain0_n32_α:
 mov rax, qword ptr [r12 + 1280]
 mov rdx, qword ptr [r12 + 1288]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp main_γ
 xchain0_n32_β:
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
 mov rdi, qword ptr [r12 + 1328]
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

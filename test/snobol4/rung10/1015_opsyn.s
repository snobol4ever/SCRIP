  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "dupl"
  .Lgvan1: .string "output"
  .Lgvan2: .string "size"
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
 mov qword ptr [r12 + 704], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "@"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "dupl"
 xchain0_n2_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+176] -> [zr+160]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lrkfn4: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 160]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx5_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n9_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "a"
 xchain0_n5_α:
# BOX IR_CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+128] -> [zr+80]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+144] -> [zr+96]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 104], rax
# marshal arg2 = producer-box slot [zr+192] -> [zr+112]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn8: .string "opsyn"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 80]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n4_α
xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 320], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx9_0:
 .quad 4
 xchain0_n7_α:
# BOX IR_CALL @(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+304] -> [zr+272]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 280], rax
# marshal arg1 = producer-box slot [zr+320] -> [zr+288]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn11: .string "@"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 272]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
xchain0_n7_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "aaaa"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n16_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "|"
 xchain0_n10_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+256] -> [zr+224]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 232], rax
# marshal arg1 = producer-box slot [zr+336] -> [zr+240]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lbynamefn15: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n12_α
xchain0_n10_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n16_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "size"
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "FAIL 1015/001: @ as binary dupl"
 xchain0_n13_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+496] -> [zr+480]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lrkfn19: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn19]
 lea rsi, [r12 + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n16_α
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx21_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n20_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "string"
 xchain0_n17_α:
# BOX IR_CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+400]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 408], rax
# marshal arg1 = producer-box slot [zr+464] -> [zr+416]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 424], rax
# marshal arg2 = producer-box slot [zr+512] -> [zr+432]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn24: .string "opsyn"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 400]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n16_α
xchain0_n17_β:
 jmp xchain0_n16_α
 xchain0_n18_α:
# BOX IR_CALL |(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+608] -> [zr+592]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 600], rax
  .section .rodata
  .Lbynamefn26: .string "|"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 592]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
xchain0_n18_β:
 jmp xchain0_n20_α
# IR_LIT_INTEGER
 xchain0_n19_α:
 mov qword ptr [r12 + 624], 6
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n20_α
.Lx27_0:
 .quad 6
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "PASS 1015_opsyn (2/2)"
 xchain0_n21_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+576] -> [zr+544]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 552], rax
# marshal arg1 = producer-box slot [zr+624] -> [zr+560]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn30: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 544]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n23_α
xchain0_n21_β:
 jmp xchain0_n20_α
# IR_ASSIGN gva
 xchain0_n22_α:
 mov rax, qword ptr [r12 + 688]
 mov rdx, qword ptr [r12 + 696]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp main_γ
 xchain0_n22_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [r12 + 656], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "FAIL 1015/002: | as unary size"
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [r12 + 656]
 mov rdx, qword ptr [r12 + 664]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp main_γ
 xchain0_n24_β:
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
 mov rdi, qword ptr [r12 + 704]
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

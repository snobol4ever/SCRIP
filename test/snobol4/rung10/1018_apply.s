  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "eq"
  .Lgvan1: .string "output"
  .Lgvan2: .string "trim"
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
 mov qword ptr [r12 + 656], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "eq"
 xchain0_n1_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+144]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lrkfn3: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 144]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx4_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "eq"
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
.Lx6_0:
 .quad 2
 xchain0_n5_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+336] -> [zr+320]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lrkfn8: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn8]
 lea rsi, [r12 + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
 xchain0_n6_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+128] -> [zr+80]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+176] -> [zr+96]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 104], rax
# marshal arg2 = producer-box slot [zr+192] -> [zr+112]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn7: .string "apply"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 80]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx10_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "FAIL 1018/002: apply(.eq,1,1) should succeed"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "FAIL 1018/001: apply(.eq,1,2) should fail"
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n8_α
.Lx13_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n11_α:
 mov rax, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
 xchain0_n13_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+304] -> [zr+256]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [zr+352] -> [zr+272]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 280], rax
# marshal arg2 = producer-box slot [zr+368] -> [zr+288]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn14: .string "apply"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 256]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n17_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "trim"
 xchain0_n15_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+544] -> [zr+528]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lrkfn19: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn19]
 lea rsi, [r12 + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "abc   "
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 640], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "PASS 1018_apply (3/3)"
 xchain0_n18_α:
# BOX IR_CALL apply(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+480]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 488], rax
# marshal arg1 = producer-box slot [zr+560] -> [zr+496]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lbynamefn19: .string "apply"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 480]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n17_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [r12 + 640]
 mov rdx, qword ptr [r12 + 648]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n17_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "abc"
 xchain0_n21_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+464] -> [zr+432]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 440], rax
# marshal arg1 = producer-box slot [zr+576] -> [zr+448]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn22: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn22]
 lea rsi, [r12 + 432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "FAIL 1018/003: apply(.trim,...)"
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp main_γ
 xchain0_n23_β:
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
 mov rdi, qword ptr [r12 + 656]
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

  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
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
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
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
 mov qword ptr [r12 + 512], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n1_α
.Lx1_0:
 .quad 1
 xchain0_n1_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+256] -> [zr+240]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lrkfn3: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 32], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad 2
 xchain0_n5_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+304] -> [zr+288]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lrkfn8: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn8]
 lea rsi, [r12 + 288]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad 2
# IR_ASSIGN gva
 xchain0_n7_α:
 mov rax, qword ptr [r12 + 64]
 mov rdx, qword ptr [r12 + 72]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx11_0:
 .quad 3
 xchain0_n9_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+352] -> [zr+336]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lrkfn13: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn13]
 lea rsi, [r12 + 336]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n10_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n10_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "STNO"
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad 1
 xchain0_n12_α:
# BOX IR_CALL GT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+128] -> [zr+96]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 104], rax
# marshal arg1 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lrkfn17: .string "GT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn17]
 lea rsi, [r12 + 96]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n15_α
.Lx18_0:
 .quad 4
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n16_α
.Lx19_0:
 .quad 6
 xchain0_n15_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+400] -> [zr+384]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lrkfn21: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn21]
 lea rsi, [r12 + 384]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n17_α
 xchain0_n16_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+496] -> [zr+480]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lrkfn23: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn23]
 lea rsi, [r12 + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n18_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n21_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "stno ok"
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "wrong"
# IR_ASSIGN global
 xchain0_n19_α:
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov rdi, qword ptr [rip + .Lx26_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n21_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov rdi, qword ptr [rip + .Lx27_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n22_α
.Lx28_0:
 .quad 5
 xchain0_n22_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+448] -> [zr+432]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lrkfn30: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn30]
 lea rsi, [r12 + 432]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je main_γ
 jmp main_γ
 xchain0_n22_β:
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
 mov rdi, qword ptr [r12 + 512]
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

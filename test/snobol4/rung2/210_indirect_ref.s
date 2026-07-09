  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "bal"
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
 mov qword ptr [r12 + 400], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 32], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 40], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "the real bal"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "bal"
 xchain0_n3_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+144] -> [zr+128]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn5: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn5]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_DEREF variable -> value
 xchain0_n4_α:
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n10_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "bal"
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
 xchain0_n7_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+304] -> [zr+288]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lrkfn10: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [r12 + 288]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
 xchain0_n8_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+96] -> [zr+64]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 72], rax
# marshal arg1 = producer-box slot [zr+160] -> [zr+80]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 88], rax
  .section .rodata
  .Lbynamefn12: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 64]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n11_α
xchain0_n8_β:
 jmp xchain0_n5_α
# IR_DEREF variable -> value
 xchain0_n9_α:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "PASS 210_indirect_ref (2/2)"
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 210/001: $string lookup"
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n10_α
# IR_ASSIGN gva
 xchain0_n13_α:
 mov rax, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
 xchain0_n15_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+256] -> [zr+224]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 232], rax
# marshal arg1 = producer-box slot [zr+320] -> [zr+240]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n16_α
xchain0_n15_β:
 jmp xchain0_n10_α
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "FAIL 210/002: $.var lookup"
# IR_ASSIGN gva
 xchain0_n17_α:
 mov rax, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp main_γ
 xchain0_n17_β:
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
 mov rdi, qword ptr [r12 + 400]
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

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
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
  call gva_register@PLT
  mov rbx, rax
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 680], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "the real bal"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rsp + 272], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 280], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "bal"
 xchain0_n3_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+272] -> [zr+240]
 mov rax, qword ptr [rsp + 272]
 mov qword ptr [rsp + 240], rax
 mov rax, qword ptr [rsp + 280]
 mov qword ptr [rsp + 248], rax
  .section .rodata
  .Lrkfn5: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn5]
 lea rsi, [rsp + 240]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_DEREF variable -> value
 xchain0_n4_α:
 mov rdi, qword ptr [rsp + 224]
 mov rsi, qword ptr [rsp + 232]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [rsp + 512], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rsp + 520], rax
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
 mov qword ptr [rsp + 304], rax
 mov qword ptr [rsp + 312], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
 xchain0_n7_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+512] -> [zr+480]
 mov rax, qword ptr [rsp + 512]
 mov qword ptr [rsp + 480], rax
 mov rax, qword ptr [rsp + 520]
 mov qword ptr [rsp + 488], rax
  .section .rodata
  .Lrkfn10: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [rsp + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
 xchain0_n8_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+208] -> [zr+160]
 mov rax, qword ptr [rsp + 208]
 mov qword ptr [rsp + 160], rax
 mov rax, qword ptr [rsp + 216]
 mov qword ptr [rsp + 168], rax
# marshal arg1 = producer-box slot [zr+304] -> [zr+176]
 mov rax, qword ptr [rsp + 304]
 mov qword ptr [rsp + 176], rax
 mov rax, qword ptr [rsp + 312]
 mov qword ptr [rsp + 184], rax
  .section .rodata
  .Lbynamefn9: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn9]
 lea rsi, [rsp + 160]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n5_α
# IR_DEREF variable -> value
 xchain0_n9_α:
 mov rdi, qword ptr [rsp + 464]
 mov rsi, qword ptr [rsp + 472]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [rsp + 640], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "PASS 210_indirect_ref (2/2)"
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rsp + 352], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 360], rax
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 210/001: $string lookup"
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n10_α
# IR_ASSIGN gva
 xchain0_n13_α:
 mov rax, qword ptr [rsp + 640]
 mov rdx, qword ptr [rsp + 648]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 624], rax
 mov qword ptr [rsp + 632], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [rsp + 352]
 mov rdx, qword ptr [rsp + 360]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
 xchain0_n15_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+400]
 mov rax, qword ptr [rsp + 448]
 mov qword ptr [rsp + 400], rax
 mov rax, qword ptr [rsp + 456]
 mov qword ptr [rsp + 408], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+416]
 mov rax, qword ptr [rsp + 544]
 mov qword ptr [rsp + 416], rax
 mov rax, qword ptr [rsp + 552]
 mov qword ptr [rsp + 424], rax
  .section .rodata
  .Lbynamefn16: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn16]
 lea rsi, [rsp + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n10_α
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [rsp + 592], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 600], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 210/002: $.var lookup"
# IR_ASSIGN gva
 xchain0_n17_α:
 mov rax, qword ptr [rsp + 592]
 mov rdx, qword ptr [rsp + 600]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 680]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 680]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

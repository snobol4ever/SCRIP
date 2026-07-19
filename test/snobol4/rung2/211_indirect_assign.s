  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "qq"
  .Lgvan1: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
  call gva_register@PLT
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
  mov qword ptr [rsp + 648], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 152], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "qq"
 xchain0_n1_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rbp + 144]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 152]
 mov qword ptr [rbp + 120], rax
  .section .rodata
  .Lrkfn3: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn3]
 lea rsi, [rbp + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rbp + 176], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 184], rax
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "x"
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 288], rax
 mov qword ptr [rbp + 296], rdx
 jmp xchain0_n5_α
# IR_ASSIGN_VAR
 xchain0_n4_α:
 mov rdi, qword ptr [rbp + 96]
 mov rsi, qword ptr [rbp + 104]
 mov rdx, qword ptr [rbp + 176]
 mov rcx, qword ptr [rbp + 184]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n3_α
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [rbp + 320], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rbp + 328], rax
 jmp xchain0_n7_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "x"
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rbp + 512], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 520], rax
 jmp xchain0_n8_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "_no_such_var_"
 xchain0_n7_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+288] -> [zr+240]
 mov rax, qword ptr [rbp + 288]
 mov qword ptr [rbp + 240], rax
 mov rax, qword ptr [rbp + 296]
 mov qword ptr [rbp + 248], rax
# marshal arg1 = producer-box slot [zr+320] -> [zr+256]
 mov rax, qword ptr [rbp + 320]
 mov qword ptr [rbp + 256], rax
 mov rax, qword ptr [rbp + 328]
 mov qword ptr [rbp + 264], rax
  .section .rodata
  .Lbynamefn8: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rbp + 240]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n6_α
 xchain0_n8_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+512] -> [zr+480]
 mov rax, qword ptr [rbp + 512]
 mov qword ptr [rbp + 480], rax
 mov rax, qword ptr [rbp + 520]
 mov qword ptr [rbp + 488], rax
  .section .rodata
  .Lrkfn11: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn11]
 lea rsi, [rbp + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rbp + 368], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 376], rax
 jmp xchain0_n12_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "FAIL 211/001: indirect assign sets named var"
# IR_DEREF variable -> value
 xchain0_n10_α:
 mov rdi, qword ptr [rbp + 464]
 mov rsi, qword ptr [rbp + 472]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 jmp xchain0_n13_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rbp + 608], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 616], rax
 jmp xchain0_n14_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "PASS 211_indirect_assign (2/2)"
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [rbp + 368]
 mov rdx, qword ptr [rbp + 376]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp main_γ
 xchain0_n13_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+416]
 mov rax, qword ptr [rbp + 448]
 mov qword ptr [rbp + 416], rax
 mov rax, qword ptr [rbp + 456]
 mov qword ptr [rbp + 424], rax
  .section .rodata
  .Lbynamefn14: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [rbp + 416]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [rbp + 608]
 mov rdx, qword ptr [rbp + 616]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp main_γ
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [rbp + 560], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain0_n16_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "FAIL 211/002: undefined indirect is null"
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rbp + 560]
 mov rdx, qword ptr [rbp + 568]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 648]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 648]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

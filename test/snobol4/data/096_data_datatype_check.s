  .intel_syntax noprefix
  .text
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lclassspec0: .string "node(val,next)"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lclassspec0]
  call record_register@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "N"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
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
  mov qword ptr [rsp + 632], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         DATA('node(val,next)')
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 152], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "node(val,next)"
 xchain0_n1_α:
# BOX IR_CALL DATA(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rbp + 144]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 152]
 mov qword ptr [rbp + 120], rax
  .section .rodata
  .Lrkfn3: .string "DATA"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn3]
 lea rsi, [rbp + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
#         N = node('hello', '')
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rbp + 256], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 264], rax
 jmp xchain0_n3_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rbp + 288], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 296], rax
 jmp xchain0_n4_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string ""
 xchain0_n4_α:
# BOX IR_CALL node(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+256] -> [zr+208]
 mov rax, qword ptr [rbp + 256]
 mov qword ptr [rbp + 208], rax
 mov rax, qword ptr [rbp + 264]
 mov qword ptr [rbp + 216], rax
# marshal arg1 = producer-box slot [zr+288] -> [zr+224]
 mov rax, qword ptr [rbp + 288]
 mov qword ptr [rbp + 224], rax
 mov rax, qword ptr [rbp + 296]
 mov qword ptr [rbp + 232], rax
  .section .rodata
  .Lrkfn7: .string "node"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn7]
 lea rsi, [rbp + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [rbp + 192]
 mov rdx, qword ptr [rbp + 200]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 176], rax
 mov qword ptr [rbp + 184], rdx
 jmp xchain0_n6_α
#         OUTPUT = REPLACE(DATATYPE(N), &LCASE, &UCASE)
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain0_n7_α
 xchain0_n7_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+464] -> [zr+432]
 mov rax, qword ptr [rbp + 464]
 mov qword ptr [rbp + 432], rax
 mov rax, qword ptr [rbp + 472]
 mov qword ptr [rbp + 440], rax
  .section .rodata
  .Lrkfn11: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn11]
 lea rsi, [rbp + 432]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 416], rax
 mov qword ptr [rbp + 424], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n8_α
#         OUTPUT = val(N)
# IR_VAR
 xchain0_n8_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain0_n10_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n9_α:
 mov rdi, qword ptr [rip + .Lx13_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 jmp xchain0_n11_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "LCASE"
 xchain0_n10_α:
# BOX IR_CALL val(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+592] -> [zr+560]
 mov rax, qword ptr [rbp + 592]
 mov qword ptr [rbp + 560], rax
 mov rax, qword ptr [rbp + 600]
 mov qword ptr [rbp + 568], rax
  .section .rodata
  .Lrkfn15: .string "val"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn15]
 lea rsi, [rbp + 560]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp main_γ
# IR_KEYWORD_SNOBOL4_read
 xchain0_n11_α:
 mov rdi, qword ptr [rip + .Lx16_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
 jmp xchain0_n13_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "UCASE"
# IR_ASSIGN global
 xchain0_n12_α:
 mov rsi, qword ptr [rbp + 544]
 mov rdx, qword ptr [rbp + 552]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "OUTPUT"
 xchain0_n13_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+416] -> [zr+352]
 mov rax, qword ptr [rbp + 416]
 mov qword ptr [rbp + 352], rax
 mov rax, qword ptr [rbp + 424]
 mov qword ptr [rbp + 360], rax
# marshal arg1 = producer-box slot [zr+496] -> [zr+368]
 mov rax, qword ptr [rbp + 496]
 mov qword ptr [rbp + 368], rax
 mov rax, qword ptr [rbp + 504]
 mov qword ptr [rbp + 376], rax
# marshal arg2 = producer-box slot [zr+512] -> [zr+384]
 mov rax, qword ptr [rbp + 512]
 mov qword ptr [rbp + 384], rax
 mov rax, qword ptr [rbp + 520]
 mov qword ptr [rbp + 392], rax
  .section .rodata
  .Lrkfn19: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn19]
 lea rsi, [rbp + 352]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n8_α
# IR_ASSIGN global
 xchain0_n14_α:
 mov rsi, qword ptr [rbp + 336]
 mov rdx, qword ptr [rbp + 344]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 jmp xchain0_n8_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 632]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 632]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

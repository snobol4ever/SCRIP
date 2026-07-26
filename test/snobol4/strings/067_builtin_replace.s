  .intel_syntax noprefix
  .text
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
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
  mov qword ptr [rsp + 296], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         OUTPUT = REPLACE('hello', 'aeiou', 'AEIOU')
# IR_LIT_STRING
xchain0_n0_α:
                        mov              qword ptr [rbp + 192], 1
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 200], rax
                        jmp              xchain0_n1_α
.Lx1_0:
                        .quad            .Lx1_0_s
.Lx1_0_s:
                        .string          "hello"
# IR_LIT_STRING
xchain0_n1_α:
                        mov              qword ptr [rbp + 224], 1
                        mov              rax, qword ptr [rip + .Lx2_0]
                        mov              qword ptr [rbp + 232], rax
                        jmp              xchain0_n2_α
.Lx2_0:
                        .quad            .Lx2_0_s
.Lx2_0_s:
                        .string          "aeiou"
# IR_LIT_STRING
xchain0_n2_α:
                        mov              qword ptr [rbp + 256], 1
                        mov              rax, qword ptr [rip + .Lx3_0]
                        mov              qword ptr [rbp + 264], rax
                        jmp              xchain0_n3_α
.Lx3_0:
                        .quad            .Lx3_0_s
.Lx3_0_s:
                        .string          "AEIOU"
xchain0_n3_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+192] -> [zr+128]
                        mov              rax, qword ptr [rbp + 192]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 200]
                        mov              qword ptr [rbp + 136], rax
# marshal arg1 = producer-box slot [zr+224] -> [zr+144]
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 144], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 152], rax
# marshal arg2 = producer-box slot [zr+256] -> [zr+160]
                        mov              rax, qword ptr [rbp + 256]
                        mov              qword ptr [rbp + 160], rax
                        mov              rax, qword ptr [rbp + 264]
                        mov              qword ptr [rbp + 168], rax
                        .section         .rodata
.Lrkfn5:                .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn5]
                        lea              rsi, [rbp + 128]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                        cmp              eax, 99
                        je               main_γ
                        jmp              xchain0_n4_α
xchain0_n3_β:
                        jmp              main_γ
# IR_ASSIGN global
xchain0_n4_α:
                        mov              rsi, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              rdi, qword ptr [rip + .Lx6_0]
                        call             NV_SET_fn@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        jmp              main_γ
.Lx6_0:
                        .quad            .Lx6_0_s
.Lx6_0_s:
                        .string          "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 296]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 296]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret

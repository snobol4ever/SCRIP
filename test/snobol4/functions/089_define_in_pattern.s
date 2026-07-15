  .intel_syntax noprefix
  .text
  .globl proc_upcase_α
proc_upcase_α:
#=======================================================================================================================
    .global proc_upcase_α
    .global proc_upcase_β
    .global proc_upcase_γ
    .global proc_upcase_ω
  sub rsp, 496
  mov [rsp + 472], rcx
  mov [rsp + 480], rdx
  mov rdi, rsp
  mov ecx, 464
  xor eax, eax
  rep stosb
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 456], rsp
proc_upcase_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_upcase_γ
# IR_KEYWORD_SNOBOL4_read
 xchain0_n1_α:
 mov rdi, qword ptr [rip + .Lx2_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_upcase_γ
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "LCASE"
# IR_KEYWORD_SNOBOL4_read
 xchain0_n2_α:
 mov rdi, qword ptr [rip + .Lx3_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_upcase_γ
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "UCASE"
 xchain0_n3_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 200]
 mov qword ptr [rsp + 136], rax
# marshal arg1 = producer-box slot [zr+224] -> [zr+144]
 mov rax, qword ptr [rsp + 224]
 mov qword ptr [rsp + 144], rax
 mov rax, qword ptr [rsp + 232]
 mov qword ptr [rsp + 152], rax
# marshal arg2 = producer-box slot [zr+240] -> [zr+160]
 mov rax, qword ptr [rsp + 240]
 mov qword ptr [rsp + 160], rax
 mov rax, qword ptr [rsp + 248]
 mov qword ptr [rsp + 168], rax
  .section .rodata
  .Lrkfn5: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn5]
 lea rsi, [rsp + 128]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je proc_upcase_γ
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp proc_upcase_γ
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp proc_upcase_γ
 xchain0_n4_β:
 jmp proc_upcase_γ
proc_upcase_res:
add rsp, 8
pop rsp
proc_upcase_β:
jmp proc_upcase_ω
proc_upcase_γ:
mov rdi, [rsp]
mov rsi, [rsp + 8]
mov rax, [rsp + 472]
lea rsp, [rsp + 496]
jmp rax
proc_upcase_ω:
mov rax, [rsp + 480]
lea rsp, [rsp + 496]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "upcase"
  .Lstartup_pp0_0: .string "s"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_upcase_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 464
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "upcase"
  .Lgvan1: .string "s"
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
  call proc_startup
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
  mov qword ptr [rsp + 456], rsp
main_α_body:
# IR_LIT_STRING
 xchain7_n0_α:
 mov qword ptr [rsp + 320], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 328], rax
 jmp xchain7_n1_α
 xchain7_n0_β:
 jmp xchain7_n3_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "hello"
 xchain7_n1_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 320]
 mov rdx, qword ptr [rsp + 328]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx10_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx10_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx10_3]
 lea rdx, [rip + .Lx10_4]
 jmp rax
.Lx10_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx10_2
.Lx10_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx10_2
.Lx10_1:
 call rt_faildescr@PLT
.Lx10_2:
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 cmp eax, 99
 je xchain7_n3_α
 jmp xchain7_n2_α
 xchain7_n1_β:
 jmp xchain7_n3_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "upcase"
# IR_ASSIGN global
 xchain7_n2_α:
 mov rsi, qword ptr [rsp + 272]
 mov rdx, qword ptr [rsp + 280]
 mov rdi, qword ptr [rip + .Lx11_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 256], rax
 mov qword ptr [rsp + 264], rdx
 jmp xchain7_n3_α
 xchain7_n2_β:
 jmp xchain7_n3_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain7_n3_α:
 mov qword ptr [rsp + 416], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 424], rax
 jmp xchain7_n4_α
 xchain7_n3_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "world"
 xchain7_n4_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 416]
 mov rdx, qword ptr [rsp + 424]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx14_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx14_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx14_3]
 lea rdx, [rip + .Lx14_4]
 jmp rax
.Lx14_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx14_2
.Lx14_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx14_2
.Lx14_1:
 call rt_faildescr@PLT
.Lx14_2:
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 cmp eax, 99
 je main_γ
 jmp xchain7_n5_α
 xchain7_n4_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "upcase"
# IR_ASSIGN global
 xchain7_n5_α:
 mov rsi, qword ptr [rsp + 368]
 mov rdx, qword ptr [rsp + 376]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp main_γ
 xchain7_n5_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 456]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 456]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

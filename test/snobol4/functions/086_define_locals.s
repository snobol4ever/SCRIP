  .intel_syntax noprefix
  .text
  .globl proc_swap_α
proc_swap_α:
#=======================================================================================================================
    .global proc_swap_α
    .global proc_swap_β
    .global proc_swap_γ
    .global proc_swap_ω
  sub rsp, 592
  mov [rsp + 568], rcx
  mov [rsp + 576], rdx
  mov [rsp + 584], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 560
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 552], rsp
proc_swap_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rsp + 160]
 mov rdx, qword ptr [rsp + 168]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [rsp + 208]
 mov rdx, qword ptr [rsp + 216]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp proc_swap_γ
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 352], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 360], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp proc_swap_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string " "
 xchain0_n8_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 320]
 mov rsi, qword ptr [rsp + 328]
 mov rdx, qword ptr [rsp + 352]
 mov rcx, qword ptr [rsp + 360]
 call str_concat_d@PLT
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp proc_swap_γ
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp proc_swap_γ
 xchain0_n10_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 288]
 mov rsi, qword ptr [rsp + 296]
 mov rdx, qword ptr [rsp + 384]
 mov rcx, qword ptr [rsp + 392]
 call str_concat_d@PLT
 mov qword ptr [rsp + 256], rax
 mov qword ptr [rsp + 264], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp proc_swap_γ
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [rsp + 256]
 mov rdx, qword ptr [rsp + 264]
 mov rdi, qword ptr [rip + .Lx12_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 jmp proc_swap_γ
 xchain0_n11_β:
 jmp proc_swap_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "OUTPUT"
proc_swap_res:
add rsp, 8
pop rsp
proc_swap_β:
jmp proc_swap_ω
proc_swap_γ:
mov rdi, [rsp]
mov rsi, [rsp + 8]
mov rax, [rsp + 568]
mov rbp, [rbp + 584]
lea rsp, [rsp + 592]
jmp rax
proc_swap_ω:
mov rax, [rsp + 576]
mov rbp, [rbp + 584]
lea rsp, [rsp + 592]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "swap"
  .Lstartup_pp0_0: .string "a"
  .Lstartup_pp0_1: .string "b"
  .Lstartup_pp0_2: .string "tmp"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad .Lstartup_pp0_1
  .quad .Lstartup_pp0_2
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 3
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_swap_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 3
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 560
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "swap"
  .Lgvan1: .string "a"
  .Lgvan2: .string "b"
  .Lgvan3: .string "tmp"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 4
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 4
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
  mov qword ptr [rsp + 552], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain13_n0_α:
 mov qword ptr [rsp + 480], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 488], rax
 jmp xchain13_n1_α
 xchain13_n0_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain13_n1_α:
 mov qword ptr [rsp + 512], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 520], rax
 jmp xchain13_n2_α
 xchain13_n1_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "world"
 xchain13_n2_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 480]
 mov rdx, qword ptr [rsp + 488]
 call rt_arg_stage@PLT
 mov edi, 1
 mov rsi, qword ptr [rsp + 512]
 mov rdx, qword ptr [rsp + 520]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx17_0]
 mov esi, 2
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx17_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx17_3]
 lea rdx, [rip + .Lx17_4]
 jmp rax
.Lx17_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx17_2
.Lx17_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx17_2
.Lx17_1:
 call rt_faildescr@PLT
.Lx17_2:
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 cmp eax, 99
 je main_γ
 jmp main_γ
 xchain13_n2_β:
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "swap"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 552]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 552]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret

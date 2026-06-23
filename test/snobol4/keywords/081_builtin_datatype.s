  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call flat_α
  xor eax, eax
  pop rbp
  ret
flat_α:
#=======================================================================================================================
    .global flat_α
    .global flat_β
    .global flat_γ
    .global flat_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
flat_α_body:
snoch0_n0_α:
# IR_LIT_S
bb1_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg2_done
 xgvarg2_β:
 jmp snoch0_n2_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "hello"
xgvarg2_done:
bb2_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn5: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn5]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp xgvarg1_done
xgvarg1_β:
 jmp snoch0_n2_α
xgvarg1_done:
# IR_KEYWORD_read
bb3_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xgvarg6_done
 xgvarg6_β:
 jmp snoch0_n2_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "LCASE"
xgvarg6_done:
# IR_KEYWORD_read
bb4_α:
 mov rdi, qword ptr [rip + .Lx9_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n2_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "UCASE"
xgvarg8_done:
bb5_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+32] -> [r12+112]
 mov rax, qword ptr [r12 + 32]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 40]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [r12+64] -> [r12+128]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 136], rax
# marshal arg2 = producer-box slot [r12+80] -> [r12+144]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn11: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 112]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je snoch0_n2_α
 jmp snoch0_n1_α
snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb6_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 96]
 mov rdx, qword ptr [r12 + 104]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_I
bb7_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg14_done
 xgvarg14_β:
 jmp snoch0_n4_α
.Lx15_0:
 .quad 42
xgvarg14_done:
bb8_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+192]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn17: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 192]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp xgvarg13_done
xgvarg13_β:
 jmp snoch0_n4_α
xgvarg13_done:
# IR_KEYWORD_read
bb9_α:
 mov rdi, qword ptr [rip + .Lx19_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xgvarg18_done
 xgvarg18_β:
 jmp snoch0_n4_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "LCASE"
xgvarg18_done:
# IR_KEYWORD_read
bb10_α:
 mov rdi, qword ptr [rip + .Lx21_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xgvarg20_done
 xgvarg20_β:
 jmp snoch0_n4_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "UCASE"
xgvarg20_done:
bb11_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+176] -> [r12+256]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [r12+208] -> [r12+272]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 280], rax
# marshal arg2 = producer-box slot [r12+224] -> [r12+288]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn23: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 256]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n3_α
snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb12_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_LIT_F
bb13_α:
 mov qword ptr [r12 + 304], 7
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 312], rax
 jmp xgvarg26_done
 xgvarg26_β:
 jmp flat_γ
.Lx27_0:
 .quad 4614253070214989087
xgvarg26_done:
bb14_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+304] -> [r12+336]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn29: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn29]
 lea rsi, [r12 + 336]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je flat_γ
 jmp xgvarg25_done
xgvarg25_β:
 jmp flat_γ
xgvarg25_done:
# IR_KEYWORD_read
bb15_α:
 mov rdi, qword ptr [rip + .Lx31_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xgvarg30_done
 xgvarg30_β:
 jmp flat_γ
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "LCASE"
xgvarg30_done:
# IR_KEYWORD_read
bb16_α:
 mov rdi, qword ptr [rip + .Lx33_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xgvarg32_done
 xgvarg32_β:
 jmp flat_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "UCASE"
xgvarg32_done:
bb17_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+320] -> [r12+400]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 408], rax
# marshal arg1 = producer-box slot [r12+352] -> [r12+416]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 424], rax
# marshal arg2 = producer-box slot [r12+368] -> [r12+432]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn35: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 400]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n5_α
snoch0_n4_β:
 jmp flat_γ
snoch0_n5_α:
bb18_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S0]
 mov rsi, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n5_β:
 jmp flat_γ
flat_β:
jmp flat_ω
flat_γ:
mov eax, 1
xor edx, edx
pop r12
ret
flat_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
.section .rodata
.S0: .string "OUTPUT"
.text

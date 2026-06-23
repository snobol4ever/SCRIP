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
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
# IR_LIT_scalar
bb3_α:
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp snoch0_n4_α
snoch0_n3_α:
bb4_α:
# IR_ASSIGN_LIT_I
 lea rdi, [rip + .S1]
 movabs rsi, 0
 call rt_gvar_assign_int@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_VAR
bb5_α:
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp flat_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "INPUT"
snoch0_n5_α:
bb6_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S2]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_LIT_S
bb7_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg8_done
 xgvarg8_β:
 jmp snoch0_n8_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string " "
xgvarg8_done:
xgvarg10_done:
bb8_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
# marshal arg1 = inline gvar-arith subexpr -> [r12+64]
 movabs rax, 80
 mov qword ptr [r12 + 96], rax
# marshal arg0 = gvar NV_GET -> [r12+128]
   lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
  .section .rodata
  .Lcallfn12: .string "SIZE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcallfn12]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 mov rcx, qword ptr [r12 + 120]
 mov rax, qword ptr [r12 + 96]
 sub rax, rcx
 mov qword ptr [r12 + 80], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 80]
 cqo
 idiv rcx
 mov qword ptr [r12 + 64], 6
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lbynamefn13: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 48]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n6_β:
 jmp snoch0_n8_α
snoch0_n7_α:
# IR_VAR
bb9_α:
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp snoch0_n9_α
 snoch0_n7_β:
 jmp snoch0_n8_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "S"
snoch0_n8_α:
xgvarg16_done:
# IR_LIT_I
bb10_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 168], rax
 jmp xgvarg17_done
 xgvarg17_β:
 jmp snoch0_n11_α
.Lx18_0:
 .quad 3
xgvarg17_done:
bb11_α:
# BOX IR_CALL REMDR(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = inline gvar-arith subexpr -> [r12+192]
   lea rdi, [rip + .S1]
 call rt_gvar_get_int@PLT
 mov qword ptr [r12 + 224], rax
 mov rcx, 1
 mov rax, qword ptr [r12 + 224]
 add rax, rcx
 mov qword ptr [r12 + 192], 6
 mov qword ptr [r12 + 200], rax
# marshal arg1 = producer-box slot [r12+160] -> [r12+208]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn20: .string "REMDR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 192]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch0_n11_α
 jmp snoch0_n10_α
snoch0_n8_β:
 jmp snoch0_n11_α
snoch0_n9_α:
# IR_LIT_S
bb12_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 248], rax
 jmp xgvarg22_done
 xgvarg22_β:
 jmp snoch0_n8_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string " "
xgvarg22_done:
xgvarg24_done:
bb13_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+272]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 280], rax
# marshal arg1 = inline gvar-arith subexpr -> [r12+288]
 movabs rax, 80
 mov qword ptr [r12 + 320], rax
# marshal arg0 = gvar NV_GET -> [r12+352]
   lea rdi, [rip + .S2]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
  .section .rodata
  .Lcallfn26: .string "SIZE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lcallfn26]
 lea rsi, [r12 + 352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 mov rcx, qword ptr [r12 + 344]
 mov rax, qword ptr [r12 + 320]
 sub rax, rcx
 mov qword ptr [r12 + 304], rax
 mov rcx, 2
 mov rax, qword ptr [r12 + 304]
 cqo
 idiv rcx
 mov qword ptr [r12 + 288], 6
 mov qword ptr [r12 + 296], rax
  .section .rodata
  .Lbynamefn27: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 272]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp xgvcat21_0d
xgvcat21_0b:
 jmp snoch0_n8_α
xgvcat21_0d:
# IR_VAR
bb14_α:
 mov rdi, qword ptr [rip + .Lx30_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xgvcat28_1d
 xgvcat28_1b:
 jmp snoch0_n8_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "S"
xgvcat28_1d:
bb15_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, qword ptr [r12 + 32]
 mov rsi, qword ptr [r12 + 40]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 call str_concat_d@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp snoch0_n12_α
 snoch0_n9_β:
 jmp snoch0_n8_α
snoch0_n10_α:
bb16_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n11_α
snoch0_n11_α:
# IR_VAR
bb17_α:
 mov rdi, qword ptr [rip + .Lx35_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xgvarg33_done
 xgvarg33_β:
 jmp snoch0_n4_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "N"
xgvarg33_done:
# IR_LIT_I
bb18_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 424], rax
 jmp xgvarg36_done
 xgvarg36_β:
 jmp snoch0_n4_α
.Lx37_0:
 .quad 0
xgvarg36_done:
bb19_α:
# BOX IR_CALL EQ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+400] -> [r12+448]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 456], rax
# marshal arg1 = producer-box slot [r12+416] -> [r12+464]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 472], rax
  .section .rodata
  .Lbynamefn39: .string "EQ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn39]
 lea rsi, [r12 + 448]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n13_α
snoch0_n11_β:
 jmp snoch0_n4_α
snoch0_n12_α:
bb20_α:
# IR_ASSIGN
 lea rdi, [rip + .S4]
 mov rsi, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n8_α
 snoch0_n12_β:
 jmp snoch0_n8_α
snoch0_n13_α:
bb21_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S4]
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n4_α
 snoch0_n13_β:
 jmp snoch0_n4_α
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
.S0: .string "TRIM"
.S1: .string "N"
.S2: .string "S"
.S3: .string "INPUT"
.S4: .string "OUTPUT"
.text

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "runtime/rt/gc_heap_rt_pseg.h"
extern uint64_t g_rtccb[32];
extern void rt_pl_pseg_switch(void);
uint64_t t_saved_rsp, t_got[10];
static unsigned char t_stack[8192] __attribute__((aligned(64)));
__asm__(".text\n.intel_syntax noprefix\n"
    ".globl t_drive\n"
    "t_drive:\n"
    "    push rbp\n    push rbx\n    push r12\n    push r13\n    push r14\n    push r15\n"
    "    mov qword ptr [rip + t_saved_rsp], rsp\n"
    "    mov rax, rdi\n    mov rdx, rsi\n"
    "    mov r8, 0x1111\n    mov r9, 0x2222\n    mov r10, 0x3333\n    mov r11, 0x4444\n"
    "    jmp rt_pl_pseg_switch@PLT\n"
    ".globl t_back\n"
    "t_back:\n"
    "    mov qword ptr [rip + t_got + 0], r8\n    mov qword ptr [rip + t_got + 8], r9\n"
    "    mov qword ptr [rip + t_got + 16], r10\n    mov qword ptr [rip + t_got + 24], r11\n"
    "    mov qword ptr [rip + t_got + 32], rbp\n    mov qword ptr [rip + t_got + 40], rbx\n"
    "    mov qword ptr [rip + t_got + 48], r12\n    mov qword ptr [rip + t_got + 56], r13\n"
    "    mov qword ptr [rip + t_got + 64], r14\n    mov qword ptr [rip + t_got + 72], r15\n"
    "    mov rsp, qword ptr [rip + t_saved_rsp]\n"
    "    pop r15\n    pop r14\n    pop r13\n    pop r12\n    pop rbx\n    pop rbp\n    ret\n"
    ".att_syntax prefix\n");
void t_drive(void *seg, void *sched);
void t_back(void);
static int fails = 0, n = 0;
static void check(const char *exit_name, const uint64_t want[10]) {
    static const char *const nm[10] = { "r8", "r9", "r10", "r11", "rbp", "rbx", "r12", "r13", "r14", "r15" };
    for (int i = 1; i < 10; i++) {
        if (i == 2 || i == 3) continue;
        n++;
        if (t_got[i] != want[i]) { fails++; printf("  FAIL %s exit: %s = %#lx, want %#lx\n", exit_name, nm[i], (unsigned long)t_got[i], (unsigned long)want[i]); }
    }
}
int main(void) {
    uint64_t want[10] = { 0xA8, g_rtccb[6], 0xA10, 0xA11, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5 };
    {
        uint64_t rec[14];
        memset(rec, 0, sizeof rec);
        rec[0] = (uint64_t)(uintptr_t)(t_stack + 4096);
        rec[1] = (uint64_t)(uintptr_t)&t_back;
        rec[2] = 0xB0; rec[3] = 0xB1; rec[4] = 0xB2; rec[5] = 0xB3; rec[6] = 0xB4; rec[7] = 0xB5;
        t_drive((void *)0, rec);
        check("choice", want);
    }
    {
        static unsigned char blk[sizeof(rt_pseg_t) + 64] __attribute__((aligned(64)));
        rt_pseg_t *b = (rt_pseg_t *)blk;
        uint64_t ra = (uint64_t)(uintptr_t)&t_back;
        memset(blk, 0, sizeof blk);
        b->orig_lo = (char *)(t_stack + 6144);
        b->len = 8;
        b->clen = 8;
        b->rbp = 0xB0; b->rbx = 0xB1; b->r12 = 0xB2; b->r13 = 0xB3; b->r14 = 0xB4; b->r15 = 0xB5;
        memcpy(rt_pseg_bytes(b), &ra, 8);
        t_drive(b, (void *)0);
        check("segment", want);
    }
    printf("RTX PLUNIFY TRANSFER: %d checks, %d fail\n", n, fails);
    return fails ? 1 : 0;
}

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
typedef struct { int32_t v; uint32_t slen; union { long i; double r; void *p; } u; } D16;
static long (*o_step)(D16); static int (*o_close)(int); static long (*o_open)(const char*,int);
static void* (*o_getpat)(const char*,int);
static unsigned long c_step, c_close, c_open, c_getpat;
long rt_defer_step(D16 a){ c_step++; if(!o_step) o_step=dlsym(RTLD_NEXT,"rt_defer_step"); return o_step(a); }
int  rt_defer_close(int a){ c_close++; if(!o_close) o_close=dlsym(RTLD_NEXT,"rt_defer_close"); return o_close(a); }
long rt_defer_open(const char*a,int b){ c_open++; if(!o_open) o_open=dlsym(RTLD_NEXT,"rt_defer_open"); return o_open(a,b); }
void* rt_defer_get_pat_fn(const char*a,int b){ c_getpat++; if(!o_getpat) o_getpat=dlsym(RTLD_NEXT,"rt_defer_get_pat_fn"); return o_getpat(a,b); }
__attribute__((destructor)) static void rpt(void){
  fprintf(stderr,"[0d] step=%lu close=%lu open=%lu get_pat_fn=%lu\n",c_step,c_close,c_open,c_getpat); }

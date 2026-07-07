#ifndef ZETA_CHOICES_H
#define ZETA_CHOICES_H
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_ALLOC_BUMP_INFINITE 0
#define ZC_ALLOC_BUMP_LIFO     1
#define ZC_ALLOC_MALLOC        2
#define ZC_ALLOC_GC            3
#ifndef ZC_ALLOC
#define ZC_ALLOC ZC_ALLOC_BUMP_LIFO
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_COL_MALLOC 0
#define ZC_COL_ARENA  1
#define ZC_COL_GC     2
#ifndef ZC_COLLECTION
#define ZC_COLLECTION ZC_COL_MALLOC
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_SELFLOAD_OFF        0
#define ZC_SELFLOAD_PLANE_CELL 1
#define ZC_SELFLOAD_ASSERT     2
#define ZC_SELFLOAD_STATIC     3
#define ZC_SELFLOAD_ALLOC      4
#ifndef ZC_SELFLOAD
#define ZC_SELFLOAD ZC_SELFLOAD_OFF
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_PORT_PLAIN        0
#define ZC_PORT_INSTRUMENTED 1
#define ZC_PORT_ALLOC        2
#ifndef ZC_PORT
#define ZC_PORT ZC_PORT_PLAIN
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_INIT_ZERO  0
#define ZC_INIT_NONE  1
#define ZC_INIT_CLONE 2
#ifndef ZC_INIT
#define ZC_INIT ZC_INIT_ZERO
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_POISON_OFF  0
#define ZC_POISON_FILL 1
#ifndef ZC_POISON
#define ZC_POISON ZC_POISON_FILL
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_TELEM_OFF 0
#define ZC_TELEM_ON  1
#ifndef ZC_TELEMETRY
#define ZC_TELEMETRY ZC_TELEM_ON
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_OVF_BOMB  0
#define ZC_OVF_GUARD 1
#ifndef ZC_OVERFLOW
#define ZC_OVERFLOW ZC_OVF_BOMB
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZC_ARENA_MB
#define ZC_ARENA_MB 1024
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_HEAP_LIBGC 0
#define ZC_HEAP_SCRIP 1
#ifndef ZC_HEAP_STRINGS
#define ZC_HEAP_STRINGS ZC_HEAP_SCRIP
#endif
#ifndef ZC_HEAP_MB
#define ZC_HEAP_MB 512
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ZC_PROMOTE_GATE 0
#define ZC_PROMOTE_ON   1
#ifndef ZC_PROMOTE
#define ZC_PROMOTE ZC_PROMOTE_GATE
#endif
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if ZC_ALLOC == ZC_ALLOC_GC
#error "ZC_ALLOC_GC is a stub until GC-3 lands (ARCH-ZETA-LOCAL-STORAGE.md section 6e)"
#endif
#if ZC_COLLECTION == ZC_COL_GC
#error "ZC_COL_GC is a stub until GC-4 lands (ARCH-ZETA-LOCAL-STORAGE.md section 6e)"
#endif
#if ZC_COLLECTION == ZC_COL_ARENA && ZC_ALLOC == ZC_ALLOC_MALLOC
#error "ZC_COL_ARENA (grow-in-place-when-top) requires a bump ZC_ALLOC arena"
#endif
#if ZC_PROMOTE == ZC_PROMOTE_ON
#error "ZC_PROMOTE_ON is a stub until the heap-promotion rung lands (ARCH-ZETA-LOCAL-STORAGE.md section 7)"
#endif
#endif

#ifndef AST_CAP_H
#define AST_CAP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
#define AST_CAP(p) (*(size_t *)((char *)(p)->c - sizeof(size_t)))
#define AST_SET_CAP(p, v) (*(size_t *)((char *)(p)->c - sizeof(size_t)) = (size_t)(v))
#ifdef __cplusplus
}
#endif
#endif

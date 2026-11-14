#ifndef BN_HEAD_RK_DIRENT_H
#define BN_HEAD_RK_DIRENT_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
struct rk_dirent { unsigned long d_ino; long d_off; unsigned short d_reclen; unsigned char d_type; char d_name[]; };
#ifdef __cplusplus
}
#endif
#endif

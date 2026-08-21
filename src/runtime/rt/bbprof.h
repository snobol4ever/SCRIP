/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef BBPROF_H
#define BBPROF_H
#ifdef __cplusplus
extern "C" {
#endif
int  bbprof_on(void);
void bbprof_record(int nid, int kind, int uid, void *lo, void *hi);
void bbprof_start(void);
void bbprof_report(void);
#ifdef __cplusplus
}
#endif
#endif

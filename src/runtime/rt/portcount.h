#ifndef PORTCOUNT_H
#define PORTCOUNT_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
int        rt_port_counts_on(void);
uint64_t * rt_port_counts_slot(int uid, int port, const char * label);
void       rt_port_counts_dump(void);
#ifdef __cplusplus
}
#endif
#endif

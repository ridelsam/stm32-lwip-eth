#ifndef __LWIP_H__
#define __LWIP_H__

#include "main.h"
#include "ethernetif.h"
#include "lwip/dhcp.h"

void lwip_lib_init(void);
void lwip_process(void);



#endif

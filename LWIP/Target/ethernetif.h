#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include <string.h>
#include "main.h"
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/timeouts.h"
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "netif/ethernet.h"
#include "lwip/memp.h"


err_t ethernetif_init(struct netif * netif);
void ethernetif_update_config(struct netif *netif);

void ethernetif_notify_conn_changed(struct netif *netif);
void ethernetif_input(struct netif * netif);
u32_t sys_now(void);

#endif

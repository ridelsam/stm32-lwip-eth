#include "lwip.h"
#include "lwip/init.h"


struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;

uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];



void lwip_lib_init(void)
{
	 /*Set IP Address :  192.168.000.222*/
	 IP_ADDRESS[0] =  192;
	 IP_ADDRESS[1] =  168;
	 IP_ADDRESS[2] =  0;
	 IP_ADDRESS[3] =  222;

	 /*Set Netmask Address :  255.255.255.000*/
	 NETMASK_ADDRESS[0]	= 255;
	 NETMASK_ADDRESS[1]	= 255;
	 NETMASK_ADDRESS[2]	= 255;
	 NETMASK_ADDRESS[3]	= 0;

	 /*Set Gatway Address :  000.000.000.000*/
	 GATEWAY_ADDRESS[0] = 0;
	 GATEWAY_ADDRESS[1] = 0;
	 GATEWAY_ADDRESS[2] = 0;
	 GATEWAY_ADDRESS[3] = 0;

	 /*Initialize lwip stack  without RTOS*/
	 lwip_init();


	 /*Initialize ip address*/
	 IP4_ADDR(&ipaddr,IP_ADDRESS[0],IP_ADDRESS[1],IP_ADDRESS[2],IP_ADDRESS[3]);

	 /*Initialize netmask address*/
	 IP4_ADDR(&netmask,NETMASK_ADDRESS[0],NETMASK_ADDRESS[1],NETMASK_ADDRESS[2],NETMASK_ADDRESS[3]);

	 /*Initialize gateway address*/
	 IP4_ADDR(&gw,GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1] , GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);


	 /*Add network interface without using RTOS*/
	 netif_add(&gnetif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);

	 /*Register default netif */
	 netif_set_default(&gnetif);

	 /*Check if link is up*/
	 if(netif_is_link_up(&gnetif))
	 {
		 netif_set_up(&gnetif);
	 }
	 else
	 {
		 netif_set_down(&gnetif);
	 }


	 /*Set link change callback*/
	 netif_set_link_callback(&gnetif,ethernetif_update_config);

}



void lwip_process(void)
{
	ethernetif_input(&gnetif);
	sys_check_timeouts();
}

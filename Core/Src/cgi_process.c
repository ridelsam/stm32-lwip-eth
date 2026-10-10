#include "cgi_process.h"
#include <string.h>
#include <stdio.h>
#include "lwip/apps/httpd.h"



const char * led_cgi_handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[]);
const tCGI LED_CGI ={"/leds.cgi",led_cgi_handler};

tCGI CGI_ARR[NUM_OF_CGIS];

const char * led_cgi_handler(int iIndex, int iNumParams, char *pcParam[],char *pcValue[])
{
	if(iIndex != 0)
	{
		return "/led_control.html";
	}

	for(int i = 0; i < iNumParams; i++)
	{
		if(strcmp(pcParam[i],"led") == 0)
		{
			if(strcmp(pcValue[i],"1") == 0)
			{
				led_toggle(GREEN_LED);
				printf("Green LED toggled.\r\n");
			}

			else if(strcmp(pcValue[i],"2") == 0)
			{
				led_toggle(BLUE_LED);
				printf("Blue LED toggled.\r\n");
			}

			else if(strcmp(pcValue[i],"3") == 0)
			{
				led_toggle(RED_LED);
				printf("Red LED toggled.\r\n");
			}
			else if(strcmp(pcValue[i],"4") == 0)
			{
				led_toggle(CUSTOM_LED);
				printf("Custom LED toggled.\r\n");
			}
		}
	}

	return "/led_control.html";
}

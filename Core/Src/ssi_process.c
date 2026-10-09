#include "lwip/apps/httpd.h"
#include <string.h>
#include "main.h"

char const * ssi_tags[] = {"TIME"};
char const ** tags =  ssi_tags;


uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
	sprintf(pcInsert, "%d", (int)HAL_GetTick());
	return strlen(pcInsert);

}



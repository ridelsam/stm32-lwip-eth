#include "lwip/apps/httpd.h"
#include <string.h>
#include "main.h"
#include "button.h"

char const * ssi_tags[] = {"TIME","SENSOR","BUTTON"};
char const ** tags =  ssi_tags;
extern ADC_HandleTypeDef hadc1;

uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
	switch(iIndex)
	{
		case 0:
			sprintf(pcInsert, "%d", (int)HAL_GetTick());
			return strlen(pcInsert);
			break;

		case 1:
			sprintf(pcInsert, "%d", (int)HAL_ADC_GetValue(&hadc1));
			return strlen(pcInsert);
			break;
            
         case 2:
			sprintf(pcInsert, "%d", (int)get_btn_state());
			return strlen(pcInsert);
			break;   
		default:
			break;

	}
   return 0;
}



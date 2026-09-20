#include "main.h"
#include "stm32f7xx_it.h"


void Systick_Handler(void)
{
	HAL_IncTick();
}

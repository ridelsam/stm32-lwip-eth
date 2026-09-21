#include "main.h"
#include "stm32f7xx_it.h"


void SysTick_Handler(void)
{
	HAL_IncTick();
}

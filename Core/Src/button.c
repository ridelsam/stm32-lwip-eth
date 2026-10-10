#include "button.h"



void button_init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin =  BTN_Pin;
	GPIO_InitStruct.Mode =  GPIO_MODE_INPUT;
	HAL_GPIO_Init(BTN_Port,&GPIO_InitStruct);
}

uint8_t get_btn_state(void)
{
	return HAL_GPIO_ReadPin(BTN_Port,BTN_Pin);
}

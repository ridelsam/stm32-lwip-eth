#include "uart.h"


UART_HandleTypeDef		huart3;

static void uart3_set_baudrate(uint32_t periph_clock, uint32_t baudrate);


int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3,(uint8_t *)&ch,1,10);
	return ch;
}

void uart3_tx_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};

	__HAL_RCC_GPIOD_CLK_ENABLE();


	GPIO_InitStruct.Pin  = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

	HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);


	__HAL_RCC_USART3_CLK_ENABLE();


	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits   = UART_STOPBITS_1;
	huart3.Init.Parity	= UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling =  UART_ONE_BIT_SAMPLE_DISABLE;

	HAL_UART_Init(&huart3);

   /*Set baudrate fix*/
	uart3_set_baudrate(16000000,115200);
}



static uint16_t compute_uart_div(uint32_t periph_clock, uint32_t baudrate)
{
	return ((periph_clock + (baudrate/2U))/baudrate);
}


static void uart3_set_baudrate(uint32_t periph_clock, uint32_t baudrate)
{
	USART3->BRR  = compute_uart_div(periph_clock,baudrate);
}


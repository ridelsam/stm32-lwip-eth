#include "main.h"
#include "leds.h"

void sysclock_config(void);

uint32_t g_hclk_freq, g_pclk1_freq,g_pclk2_freq;

int main(void)
{
	HAL_Init();
	sysclock_config();
	leds_init();

	g_hclk_freq = HAL_RCC_GetHCLKFreq();
	g_pclk1_freq = HAL_RCC_GetPCLK1Freq();
	g_pclk2_freq = HAL_RCC_GetPCLK2Freq();

	led_on(GREEN_LED);
	led_on(BLUE_LED);
	led_on(RED_LED);


	while(1)
	{

	}
}


void sysclock_config(void)
{
	RCC_OscInitTypeDef		RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef		RCC_ClkInitStruct = {0};

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState		 = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM		 = 12;
	RCC_OscInitStruct.PLL.PLLN		 = 192;
	RCC_OscInitStruct.PLL.PLLQ		 = 2;
	RCC_OscInitStruct.PLL.PLLP		 = RCC_PLLP_DIV2;

	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/*Activate the over drive mode */
	HAL_PWREx_EnableOverDrive();


	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
								  |RCC_CLOCKTYPE_PCLK1 |RCC_CLOCKTYPE_PCLK2;

	RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	HAL_RCC_ClockConfig(&RCC_ClkInitStruct,FLASH_LATENCY_6);

}

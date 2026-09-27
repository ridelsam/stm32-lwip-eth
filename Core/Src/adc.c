#include "adc.h"

#define SENSOR_PIN		(1U<<4)
#define SENSOR_PORT		GPIOA


ADC_HandleTypeDef hadc1;


void adc1_pa4_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
    ADC_ChannelConfTypeDef sConfig 	  = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_ADC1_CLK_ENABLE();

	/*Set PA4 as analog pin*/
	GPIO_InitStruct.Pin  = SENSOR_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(SENSOR_PORT,&GPIO_InitStruct);


	/*Configure ADC parameters*/

	hadc1.Instance =  ADC1;
	hadc1.Init.ClockPrescaler =  ADC_CLOCK_SYNC_PCLK_DIV8;
	hadc1.Init.Resolution	  =  ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode   =  ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode	= DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	HAL_ADC_Init(&hadc1);

	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank    = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime =  ADC_SAMPLETIME_480CYCLES;

	HAL_ADC_ConfigChannel(&hadc1,&sConfig);


}

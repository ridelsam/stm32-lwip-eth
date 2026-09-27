#include "ethernetif.h"

#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/timeouts.h"
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "netif/ethernet.h"
#include "lwip/memp.h"
#include <string.h>




ETH_HandleTypeDef heth;

void HAL_ETH_MspInit( ETH_HandleTypeDef * ethHandle )
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(ethHandle->Instance ==  ETH)
	{
		/*See page89 of datasheet for these pins */

		/**ETH GPIO Configuration
		PC1     ------> ETH_MDC
		PA1     ------> ETH_REF_CLK
		PA2     ------> ETH_MDIO
		PA7     ------> ETH_CRS_DV
		PC4     ------> ETH_RXD0
		PC5     ------> ETH_RXD1
		PB13     ------> ETH_TXD1
		PG11     ------> ETH_TX_EN
		PG13     ------> ETH_TXD0
		*/

		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();

		__HAL_RCC_ETH_CLK_ENABLE();


		GPIO_InitStruct.Pin   = GPIO_PIN_1 |GPIO_PIN_4|GPIO_PIN_5;
		GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = GPIO_PIN_1 |GPIO_PIN_2|GPIO_PIN_7;
		GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = GPIO_PIN_13;
		GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


		GPIO_InitStruct.Pin   = GPIO_PIN_11 |GPIO_PIN_13;
		GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		/*Set ETH interrupt priority*/

		HAL_NVIC_SetPriority(ETH_IRQn,0,0);

		/*Enable ETH interrupt*/
		HAL_NVIC_EnableIRQ(ETH_IRQn);

	}

}



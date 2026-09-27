#include "ethernetif.h"

#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/timeouts.h"
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "netif/ethernet.h"
#include "lwip/memp.h"
#include <string.h>


__ALIGN_BEGIN  ETH_DMADescTypeDef DMARxDscrTab[ETH_RXBUFNB]  __ALIGN_END;
__ALIGN_BEGIN  ETH_DMADescTypeDef DMATxDscrTab[ETH_TXBUFNB]  __ALIGN_END;
__ALIGN_BEGIN  uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END;
__ALIGN_BEGIN  uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END;


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

static void low_level_init(struct netif *netif)
{
  uint32_t regvalue = 0;

  HAL_StatusTypeDef 	hal_eth_init_status;

  uint8_t MACAddr[6];

  heth.Instance = ETH;
  heth.Init.AutoNegotiation =  ETH_AUTONEGOTIATION_ENABLE;
  heth.Init.Speed =  ETH_SPEED_100M;
  heth.Init.DuplexMode =  ETH_MODE_FULLDUPLEX;
  heth.Init.PhyAddress = LAN8742A_PHY_ADDRESS;

  MACAddr[0] = 0x00;
  MACAddr[1] = 0xAB;
  MACAddr[2] = 0xCD;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;

  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.RxMode  =  ETH_RXPOLLING_MODE;

  heth.Init.ChecksumMode =  ETH_CHECKSUM_BY_HARDWARE;
  heth.Init.MediaInterface =  ETH_MEDIA_INTERFACE_RMII;

  hal_eth_init_status = HAL_ETH_Init(&heth);

  if(hal_eth_init_status ==  HAL_OK)
  {
	  netif->flags |= NETIF_FLAG_LINK_UP;
  }

  HAL_ETH_DMATxDescListInit(&heth, DMATxDscrTab, &Tx_Buff[0][0],ETH_TXBUFNB);

  HAL_ETH_DMARxDescListInit(&heth, DMARxDscrTab, &Rx_Buff[0][0],ETH_RXBUFNB);

  /*set MAC address len*/
  netif->hwaddr_len =  ETH_HWADDR_LEN;

  netif->hwaddr[0] = heth.Init.MACAddr[0];
  netif->hwaddr[1] = heth.Init.MACAddr[1];
  netif->hwaddr[2] = heth.Init.MACAddr[2];
  netif->hwaddr[3] = heth.Init.MACAddr[3];
  netif->hwaddr[4] = heth.Init.MACAddr[4];
  netif->hwaddr[5] = heth.Init.MACAddr[5];


   /*Set MTU : Maximum transfer unit*/
  netif->mtu = 1500;

  /*Enable broadcasting and ARP*/
  netif->flags  |= NETIF_FLAG_BROADCAST |NETIF_FLAG_ETHARP;

  /*Start MAC and DMA transmission and reception*/

  HAL_ETH_Start(&heth);


  /*Enable link down interupt*/
  HAL_ETH_ReadPHYRegister(&heth,PHY_ISFR,&regvalue);
  regvalue |= PHY_ISFR_INT4;

  HAL_ETH_WritePHYRegister(&heth,PHY_ISFR,regvalue);

  HAL_ETH_ReadPHYRegister(&heth,PHY_ISFR,&regvalue);



}

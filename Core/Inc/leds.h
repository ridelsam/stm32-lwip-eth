#ifndef __LEDS_H_
#define __LEDS_H_

#include "main.h"

#define USER_LED1_PIN        (1U<<0)  // Green
#define USER_LED2_PIN        (1U<<7)  // Blue   
#define USER_LED3_PIN        (1U<<14) // Red
#define USER_LED4_PIN        (1U<<15) // Custom

#define GREEN_LED             USER_LED1_PIN
#define BLUE_LED              USER_LED2_PIN
#define RED_LED               USER_LED3_PIN     
#define CUSTOM_LED            USER_LED4_PIN 

#define LED_GPIO_PORT        GPIOB

void leds_init(void);
void led_off(uint32_t led);
void led_on(uint32_t led);
void led_toggle(uint32_t led);
uint8_t led_is_on(uint32_t led);



#endif /* __LEDS_H_ */

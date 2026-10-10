#ifndef __BUTTON_H__
#define __BUTTON_H__
#include "main.h"

#define BTN_Pin		GPIO_PIN_13
#define	BTN_Port	GPIOC

void button_init(void);
uint8_t get_btn_state(void);

#endif

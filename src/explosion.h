#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "stm32f10x.h"

#define EXP_PIN_STACK 20
#define EXP_KEY_CODE_ENTER 10

#define SIZE_LIST_PASS 6

#define PIN_DETECTOR 0
#define PORT_DETECTOR GPIOB

#define __RCC(a) RCC_APB2Periph_GPIOB // TODO kostil
#define E_RCC(a) __RCC(a)

#define __PIN(a) GPIO_Pin_ ## a
#define E_PIN(a) __PIN(a)


void explosion_init(void);
void setting_password(void);
void setting_time(void);

void explotion_check_pin(void);
void explotion_statem_count_down(void);


#endif
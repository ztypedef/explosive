#ifndef LED4X_H
#define LED4X_H
#include "stm32f10x.h"

#define LED4X_SPI_SD                   		SPI2
#define LED4X_GPIO_CS                  		GPIOB
#define LED4X_RCC_APB2Periph_GPIO_CS   		RCC_APB2Periph_GPIOB
#define LED4X_GPIO_SPI_SD              		GPIOB
#define LED4X_GPIO_Pin_CS              		GPIO_Pin_12
#define LED4X_GPIO_Pin_SPI_SD_SCK      		GPIO_Pin_13
#define LED4X_GPIO_Pin_SPI_SD_MISO     		GPIO_Pin_14
#define LED4X_GPIO_Pin_SPI_SD_MOSI     		GPIO_Pin_15
#define LED4X_RCC_APBPeriphClockCmd_SPI_SD  RCC_APB1PeriphClockCmd
#define LED4X_RCC_APBPeriph_SPI_SD     		RCC_APB1Periph_SPI2
#define LED4X_SPI_BaudRatePrescaler_SPI_SD  SPI_BaudRatePrescaler_2


/**************************
		 _ _a_
		|	  |
		|f	  |b
		|_ _g_|
		|	  |
		|e	  |c
		|_ _d_|  ,dp

	 _    _    _    _
	|_|  |_|  |_|  |_|
	|_| .|_|. |_|. |_|.
	 0    1    2    3
		
**************************/

#define LED_PIN_0  1
#define LED_PIN_1  2
#define LED_PIN_2  3
#define LED_PIN_3  0


#define LED_PIN_A  (1 << 2)
#define LED_PIN_B  (1 << 0)
#define LED_PIN_C  (1 << 4)
#define LED_PIN_D  (1 << 6)
#define LED_PIN_E  (1 << 7)
#define LED_PIN_F  (1 << 1)
#define LED_PIN_G  (1 << 3)
#define LED_PIN_DP (1 << 5)


void led_init(void);
void led_update(void);
void led_char(char ch, uint8_t n);
void led_other_pin(uint8_t mask);
void led_clear();
void led_clear_pos(uint8_t pos);
#endif
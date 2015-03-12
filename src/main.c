#include "main.h"
#include "buart.h"
//#include "evaluator.h"
#include "button.h"
#include "stdio.h"
//#include "sdcard.h"
#include "LED4X.h"
#include "explosion.h"
#include "pflash.h"

/* used timer
TIM1 TIM2 TIM3 TIM6 TIM7 TIM15 TIM16

TIM2 - sound
TIM6 - evaluator
TIM7 - sdcard
TIM3 - explosion

TIM1 TIM15 TIM16
*/
int putchar (int c)
{
    if(c == '\n') 
      putchar('\r');
    while(( USART2->SR & USART_SR_TXE) == 0);
    USART2->DR = c;
    return 0;
}

int main()
{
    //FLASH_SetLatency(FLASH_Latency_1);
    /*
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_4);
    RCC_PLLCmd(ENABLE);

    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    

    while(RCC_GetSYSCLKSource() != 0x08);
    */
    
    FLASH_SetLatency(FLASH_Latency_1);


	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
	RCC_PLLCmd(ENABLE);
	
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    
	uart_init();
	//printf("%s\n", "START");
	button_init();
	//ev_init();
   	led_init();
	explosion_init();
	setting_password();
	setting_time();
	//

	
    led_other_pin(0x4);
	led_char(0x30 + 0, 0);
    led_char(0x30 + 0, 1);
   	led_char(0x30 + 0, 2);
 	led_char(0x30 + 0, 3);
	while(!explotion_get_state_getectors())
	{
		button_update();
        //ev_idle();
		led_update();
	}
	explotion_count_down();
	
	while(1)
    {
        button_update();
        //ev_idle();
		led_update();
		
		explotion_check_pin();
        //uint16_t bt = button_get_key();
		//led_char(0x30 + bt, 0);
    }
}

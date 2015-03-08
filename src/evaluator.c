#include "main.h"
#include "evaluator.h"
#include "stdio.h"

volatile long ev_count = 0;

void ev_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0x1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 0x8000;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0x0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);

}

inline void ev_idle(void)
{
	ev_count++;
    if(ev_count < 0) while(1);
}

void TIM6_DAC_IRQHandler(void)
{
    //disk_timerproc();
	//printf("%i\n", ev_count);
    ev_count = 0;
    
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
}
#include "explosion.h"
#include "LED4X.h"
#include "stdio.h"
#include "button.h"
#include "string.h"

void explosion_idle(void){};

struct explosion_time
{
	int8_t minutes;
	int8_t second;
}explosion_time;
struct explosion_time exp_time;

struct sexplotion_list_password
{
	uint8_t *pass[SIZE_LIST_PASS];
	uint8_t size[SIZE_LIST_PASS];
	void (*f[SIZE_LIST_PASS])(void);
}sexplotion_list_password;
struct sexplotion_list_password explotion_list_password;

volatile uint8_t timer = 0, eled = 0;
uint8_t e_pin[4] = {0xff, 0xff, 0xff, 0xff};
uint8_t e_stack[EXP_PIN_STACK], e_stack_pos = 0;
void (*f_state_mashine)(void) = &explosion_idle;

void explosion_pass(void);
void explotion_statem_count_down(void);

void explosion_happy_end(void);
void explosion_the_end(void);
void explosion_pass_error(void);


void explosion_detector_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(E_RCC(PORT_DETECTOR), ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = E_PIN(PIN_DETECTOR);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(PORT_DETECTOR, &GPIO_InitStruct);
}

void explosion_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
		
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1000;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStruct.TIM_Period = 24000;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	//f_state_mashine = &explosion_idle;
	exp_time.minutes = 0xFF;
	exp_time.second = 0xFF;
	
	memcpy(&explotion_list_password, 0, sizeof(struct sexplotion_list_password));
	explotion_list_password.f[0] = &explosion_happy_end;
	explotion_list_password.pass[0] = e_pin;
    
	explosion_detector_init();
}



void setting_password()
{
	uint8_t cpin = 0;
	led_char('P', 0);
    led_char('A', 1);
   	led_char('S', 2);
 	led_char('S', 3);
	eled = 1;
	f_state_mashine = &explosion_pass;
	timer = 0;
	button_stack_clear();
	TIM_Cmd(TIM3, ENABLE);
	while(timer < 10)
	{
		for(int i = 0; i < 100; i++)
		{
			led_update();
			button_update();
		}
		int16_t key = button_get_key();
		if(key >= 0)
		{
			led_clear();
			timer = 5;
			if(cpin < 4) e_pin[cpin++] = key;
            explotion_list_password.size[0] = cpin;
			led_char(0x30 + e_pin[0], 0);
			led_char(0x30 + e_pin[1], 1);
			led_char(0x30 + e_pin[2], 2);
			led_char(0x30 + e_pin[3], 3);
			if(key == EXP_KEY_CODE_ENTER) break;
		}
	}
	TIM_Cmd(TIM3, DISABLE);
	if(cpin != 4)
	{
		e_pin[0] = 1;
		e_pin[1] = 2;
		e_pin[2] = 3;
		e_pin[3] = 9;
		explotion_list_password.size[0] = 4;
	}
	led_other_pin(0);
}

void setting_time()
{
	uint8_t ckey = 0;
	led_char('T', 0);
    led_char('I', 1);
   	led_char('M', 2);
 	led_char('E', 3);
	eled = 2;
	f_state_mashine = &explosion_pass;
	timer = 0;
	TIM_Cmd(TIM3, ENABLE);
	button_stack_clear();
	while(timer < 10)
	{
		for(int i = 0; i < 100; i++)
		{
			led_update();
			button_update();
		}
		int16_t key = button_get_key();
		if(key >= 0)
		{
			led_clear();
			timer = 5;
			switch(ckey)
			{
				case 0: exp_time.minutes = key * 10; break;
				case 1: exp_time.minutes += key; break;
				case 2: exp_time.second = key * 10; break;
				case 3: exp_time.second += key; break;
				default: break;
			}
			if(ckey < 4) ckey++;
			led_char(0x30 +exp_time.minutes / 10, 0);
			led_char(0x30 +exp_time.minutes % 10, 1);
			led_char(0x30 +exp_time.second / 10, 2);
			led_char(0x30 +exp_time.second % 10, 3);
			if(key == EXP_KEY_CODE_ENTER) break;
		}
	}
	if(ckey != 4)
	{
		exp_time.minutes = 41;
		exp_time.second = 0;
	}
	TIM_Cmd(TIM3, DISABLE);
	led_other_pin(0);
}

void explotion_count_down()
{
	button_stack_clear();
	f_state_mashine = &explotion_statem_count_down;
	TIM_Cmd(TIM3, ENABLE);
}


/***********STATE MASHINE*******************/

void explosion_pass()
{
	static uint8_t k = 0;
	timer++;
    k ^= eled;
    led_other_pin(k);
}

void explotion_statem_count_down()
{
	if(--exp_time.second <= 0)
	{
		exp_time.second = 60;
		if(--exp_time.minutes < 0)
		{
			//TODO EXPLOTION
			TIM_Cmd(TIM3, DISABLE);
			led_clear();
			led_other_pin(0x8);
			return;
		}
	}
	
	led_char(0x30 + exp_time.minutes / 10, 0);
	led_char(0x30 + exp_time.minutes % 10, 1);
	led_char(0x30 + exp_time.second / 10, 2);
	led_char(0x30 + exp_time.second % 10, 3);
	
}

/***********END MASHINE*********************/

/************DETECTOR********************************/

uint8_t explotion_get_state_getectors()
{
	uint8_t data = ((GPIO_ReadInputData(PORT_DETECTOR) >> PIN_DETECTOR) & 0x1);
	return data;
}

/*************END DETECTOR*******************************/

/***************CHECK PIN****************************/
void explotion_stack_clear()
{
	e_stack_pos = 0;
}

void explotion_key_push(uint8_t key)
{
	if(e_stack_pos < EXP_PIN_STACK)
	{
		e_stack[e_stack_pos] = key;
		e_stack_pos++;
	}
	else
	{
		for(int i = 0; i < EXP_PIN_STACK - 1; i++)
		{
			e_stack[i] = e_stack[i + 1];
		}
		e_stack[EXP_PIN_STACK - 1] = key;
	}
}

void explotion_key_check()
{
	if(e_stack_pos == 0) return;
	for(int i = 0; i < SIZE_LIST_PASS; i++)
	{
		if(explotion_list_password.size[i] == e_stack_pos)
		{
			if(memcmp(&e_stack, explotion_list_password.pass[i], e_stack_pos) == 0)
			{
				explotion_list_password.f[i]();
				return;
			}
		}
		
	}
	explosion_pass_error();
}

void explotion_check_pin()
{
	int16_t key = button_get_key();
	if(key >= 0)
	{
		if(key == EXP_KEY_CODE_ENTER)
		{
			explotion_key_check();
			explotion_stack_clear();
			return;
		}
		explotion_key_push(key);
	}
}
/*******************************************/

void explosion_happy_end(void)
{
	led_other_pin(1);
	TIM_Cmd(TIM3, DISABLE);
}

void explosion_the_end(void)
{
	
}

void explosion_pass_error(void)
{
	//TIM_TimeBaseInitStruct.TIM_Period = 1000;
	TIM3->ARR = 2000;
}

void TIM3_IRQHandler(void)
{
    f_state_mashine();
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}
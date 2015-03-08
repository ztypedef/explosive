#include "main.h"
#include "button.h"

/******************************************

 PB3  |  7  |  9  |  8  |
 -----------------------|
 PB4  |  4  |  6  |  5  |
 -----------------------|
 PB5  |  1  |  3  |  2  |
 -----------------------|
 PB6  |     |  ,  |  0  |
 -----------------------|
      | PB7 | PB8 | PB9 |

******************************************/

uint16_t button_mask = 0;
uint8_t button_stack[SIZE_BUTTON_STACK];
uint8_t button_stack_pos = 0;

void button_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

void enable_line(char n)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
	switch(n)
	{
		case 0:
			GPIO_SetBits(GPIOB, GPIO_Pin_3);
			break;
		case 1:
			GPIO_SetBits(GPIOB, GPIO_Pin_4);
			break;
		case 2:
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
			break;
		case 3:
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
			break;
		default:
			break;
	}
}

void button_push(uint8_t key)
{
	if(button_stack_pos < SIZE_BUTTON_STACK)
	{
		button_stack[button_stack_pos] = key;
		button_stack_pos++;
	}
	else
	{
		for(int i = 0; i < SIZE_BUTTON_STACK - 1; i++)
		{
			button_stack[i] = button_stack[i + 1];
		}
		button_stack[SIZE_BUTTON_STACK - 1] = key;
	}
}

int16_t button_pop()
{
	if(button_stack_pos > 0)
	{
		button_stack_pos--;
		return button_stack[button_stack_pos];
	}
	return -1;
}

void button_stack_clear(void)
{
	button_stack_pos = 0;
}

void button_compare_fail(uint16_t mask)
{
	for(uint16_t i = 0; i < AMOUNT_KEY; i++)
	{
		if(((mask >> i) & 0x1) < ((button_mask >> i) & 0x1))
		{
			button_push(i);
		}
	}
}

void button_update(void)
{
	static char l_count = 0;
	uint16_t data, tmpmask = button_mask;
	enable_line(l_count);
	data = (GPIO_ReadInputData(GPIOB) >> 7) & 0x7;
	
	switch(l_count)
	{
		case 0:
			tmpmask &= ~((1 << 7) | (1 << 8) | (1 << 9));
			tmpmask |= (data & 0x1 ? 1 : 0) << 7;
			tmpmask |= (data & 0x2 ? 1 : 0) << 9;
			tmpmask |= (data & 0x4 ? 1 : 0) << 8;
			break;
		case 1:
			tmpmask &= ~((1 << 4) | (1 << 5) | (1 << 6));
			tmpmask |= (data & 0x1 ? 1 : 0) << 4;
			tmpmask |= (data & 0x2 ? 1 : 0) << 6;
			tmpmask |= (data & 0x4 ? 1 : 0) << 5;
			break;
		case 2:
			tmpmask &= ~((1 << 1) | (1 << 2) | (1 << 3));
			tmpmask |= (data & 0x1 ? 1 : 0) << 1;
			tmpmask |= (data & 0x2 ? 1 : 0) << 3;
			tmpmask |= (data & 0x4 ? 1 : 0) << 2;
			break;
		case 3:
			tmpmask &= ~((1 << 0) | (1 << 10));
			tmpmask |= (data & 0x4 ? 1 : 0) << 0;
			tmpmask |= (data & 0x2 ? 1 : 0) << 10;
			break;
		default:
			l_count = 0;
			return;
	}
    button_compare_fail(tmpmask);
	button_mask = tmpmask;
	if(++l_count > AMOUNT_LINE) l_count = 0;
}

int16_t button_get_key(void)
{
    return button_pop();
}

int16_t button_get_dec(void)
{
	uint8_t n = AMOUNT_KEY;
	while(n--)
	{
		if(button_mask & 0x1)
		{
			if(button_mask >> 1) return -1;
			return AMOUNT_KEY - n - 1;
		}
		else
		{
			button_mask >>= 1;
		}
	}
	return -2;
}
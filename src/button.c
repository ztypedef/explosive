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
	button_mask = tmpmask;
	if(++l_count > AMOUNT_LINE) l_count = 0;
}

int button_get_key(void)
{
    return button_mask;
}

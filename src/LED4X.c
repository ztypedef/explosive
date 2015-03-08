#include "LED4X.h"

#define SELECT()        GPIO_ResetBits(LED4X_GPIO_CS, LED4X_GPIO_Pin_CS)
#define DESELECT()      GPIO_SetBits(LED4X_GPIO_CS, LED4X_GPIO_Pin_CS)

uint8_t led_buff[4] = { 0, 0, 0, 0};
uint8_t led_pos[4]  = { LED_PIN_0, LED_PIN_1, LED_PIN_2, LED_PIN_3};

typedef struct sled_data
{
	uint8_t led_data_2;
	uint8_t led_data_1;
}sled_data;
sled_data led_data;

void led_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED4X_RCC_APB2Periph_GPIO_CS, ENABLE);
	LED4X_RCC_APBPeriphClockCmd_SPI_SD(LED4X_RCC_APBPeriph_SPI_SD, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = LED4X_GPIO_Pin_CS;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED4X_GPIO_CS, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin   = LED4X_GPIO_Pin_SPI_SD_SCK | LED4X_GPIO_Pin_SPI_SD_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(LED4X_GPIO_SPI_SD, &GPIO_InitStructure);
	/* Configure MISO as Input with internal pull-up */
	GPIO_InitStructure.GPIO_Pin   = LED4X_GPIO_Pin_SPI_SD_MISO;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(LED4X_GPIO_SPI_SD, &GPIO_InitStructure);

	/* SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // 72000kHz/256=281kHz < 400kHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0;

	SPI_Init(LED4X_SPI_SD, &SPI_InitStructure);
	SPI_CalculateCRC(LED4X_SPI_SD, DISABLE);
	SPI_Cmd(LED4X_SPI_SD, ENABLE);

	/* drain SPI */
	while (SPI_I2S_GetFlagStatus(LED4X_SPI_SD, SPI_I2S_FLAG_TXE) == RESET) { ; }

    SELECT();
}

void led_send_data(uint16_t data)
{
	SPI_I2S_SendData(LED4X_SPI_SD, (data >> 0) & 0xff);
	while(SPI_I2S_GetFlagStatus(LED4X_SPI_SD, SPI_I2S_FLAG_BSY) == SET);

	SPI_I2S_SendData(LED4X_SPI_SD, (data >> 8) & 0xff);
	while(SPI_I2S_GetFlagStatus(LED4X_SPI_SD, SPI_I2S_FLAG_BSY) == SET);

	DESELECT();
    SELECT();
}

inline void led_update(void)
{
	static uint8_t posx = 0;
	if((++posx) > 0x3) posx = 0x0;
	led_data.led_data_2 &= 0xF0;
	led_data.led_data_2 |= (1 << posx);
	led_data.led_data_1 = led_buff[posx];
	led_send_data(*(uint16_t*) &(led_data));
}

uint8_t led_ascii[] = 
{
/*0*/	LED_PIN_A | LED_PIN_B | LED_PIN_C | LED_PIN_D | LED_PIN_E | LED_PIN_F,
/*1*/	LED_PIN_B | LED_PIN_C,
/*2*/	LED_PIN_A | LED_PIN_B | LED_PIN_D | LED_PIN_E | LED_PIN_G,
/*3*/	LED_PIN_A | LED_PIN_B | LED_PIN_C | LED_PIN_D | LED_PIN_G,
/*4*/	LED_PIN_B | LED_PIN_C | LED_PIN_F | LED_PIN_G,
/*5*/	LED_PIN_A | LED_PIN_C | LED_PIN_D | LED_PIN_F | LED_PIN_G,
/*6*/	LED_PIN_A | LED_PIN_C | LED_PIN_D | LED_PIN_E | LED_PIN_F | LED_PIN_G,
/*7*/	LED_PIN_A | LED_PIN_B | LED_PIN_C,
/*8*/	LED_PIN_A | LED_PIN_B | LED_PIN_C | LED_PIN_D | LED_PIN_E | LED_PIN_F | LED_PIN_G,
/*9*/	LED_PIN_A | LED_PIN_B | LED_PIN_C | LED_PIN_D | LED_PIN_F | LED_PIN_G,

/*A*/	LED_PIN_A | LED_PIN_B | LED_PIN_C | LED_PIN_E | LED_PIN_F | LED_PIN_G,
/*P*/	LED_PIN_A | LED_PIN_B | LED_PIN_E | LED_PIN_F | LED_PIN_G,
/*S*/	/**S == 5**/
/*T*/	LED_PIN_A | LED_PIN_E | LED_PIN_F,
/*I*/	LED_PIN_E | LED_PIN_F,
/*M*/	LED_PIN_A | LED_PIN_B | LED_PIN_C | LED_PIN_E | LED_PIN_F,
/*E*/	LED_PIN_A | LED_PIN_D | LED_PIN_E | LED_PIN_F | LED_PIN_G,
};

void led_char(char ch, uint8_t n)
{
	if(ch >= 0x30 && ch <= 0x3A)
	{
		led_buff[led_pos[n]] = ~led_ascii[ch - 0x30];
		return;
	}
	
	switch(ch)
	{
		case 0:
			led_buff[led_pos[n]] = 0xff;
			return;
		case 'a':
		case 'A':
			led_buff[led_pos[n]] = ~led_ascii[10];
			return;
			
		case 'e':
		case 'E':
			led_buff[led_pos[n]] = ~led_ascii[15];
			return;
			
		case 'i':
		case 'I':
			led_buff[led_pos[n]] = ~led_ascii[13];
			return;
			
		case 'm':
		case 'M':
			led_buff[led_pos[n]] = ~led_ascii[14];
			return;
			
		case 'p':
		case 'P':
			led_buff[led_pos[n]] = ~led_ascii[11];
			return;
			
		case 's':
		case 'S':
			led_buff[led_pos[n]] = ~led_ascii[5];
			return;
		
		case 't':
		case 'T':
			led_buff[led_pos[n]] = ~led_ascii[12];
			return;
	}
}

void led_other_pin(uint8_t mask)
{
	led_data.led_data_2 = (mask & 0xf) << 4;
}

void led_clear()
{
	led_buff[0] = 0xff;
	led_buff[1] = 0xff;
	led_buff[2] = 0xff;
	led_buff[3] = 0xff;
}
void led_clear_pos(uint8_t pos)
{
	led_buff[led_pos[pos]] = 0xff;
}


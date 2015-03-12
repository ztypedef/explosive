#include "stm32f10x.h"
#include "sdcard.h"
#include "ff.h"

char buff1[1024];
FRESULT result;

void sdcard_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0x1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 0x8000;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0x0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}


void TIM4_IRQHandler(void)
{
    disk_timerproc();
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
}

void load_sound()
{
	FATFS FATFS_Obj;
    FIL file;
    result = f_mount(0, &FATFS_Obj);

    UINT nRead;
    result = f_open(&file, "0:/1.txt", FA_OPEN_EXISTING | FA_READ);
    if (result == FR_OK)
    {
            f_read(&file, &buff1, 4, &nRead);
			//f_read(&file, &buff2, 4, &nRead);
    }

    f_close(&file);
    printf("%s\n", buff1);
    //printf("%s\n", buff2);
    
    // Чтение файла порциями по 2048 байт
    /*
    do
    {
        result = f_read(&file, (BYTE *)temp_buff, 4096, (UINT*)&read_size);
        full_size += read_size;
    }
    while ((result == FR_OK) && (read_size == 4096));
*/
	
}
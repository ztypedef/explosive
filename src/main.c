#include "main.h"
#include "buart.h"
#include "evaluator.h"
#include "button.h"
#include "stdio.h"
#include "ff.h"

int putchar (int c)
{
    if(c == '\n') 
      putchar('\r');
    while(( USART2->SR & USART_SR_TXE) == 0);
    USART2->DR = c;
    return 0;
}


char buff[1024];
FRESULT result;


int main()
{
	button_init();
	uart_init();
    ev_init();
	printf("%s\n", "START");

    
    FATFS FATFS_Obj;
    FIL file;
    
    // Монтирование диска
    result = f_mount(0, &FATFS_Obj);
    
    //scan_files("0:/");
    // Открытие файла
    UINT nRead;
    result = f_open(&file, "0:/1.txt", FA_OPEN_EXISTING | FA_READ);
    if (result == FR_OK)
    {
            f_read(&file, &buff, 1024, &nRead);
            f_close(&file);
    }
    // Закрытие файла
    result = f_close(&file);
    printf("%s\n", buff);
    
    // Чтение файла порциями по 2048 байт
    /*
    do
    {
        result = f_read(&file, (BYTE *)temp_buff, 4096, (UINT*)&read_size);
        full_size += read_size;
    }
    while ((result == FR_OK) && (read_size == 4096));
*/
	
	while(1)
    {
        button_update();
        ev_idle();
    }
}

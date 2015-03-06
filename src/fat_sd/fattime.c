/* Martin Thomas 4/2009 */

#include "integer.h"
#include "fattime.h"


DWORD get_fattime (void)
{

// возвращаем константное время
    return ((DWORD)(2015 - 1980) << 25)   // год
            | ((DWORD)9 << 21)            // месяц
            | ((DWORD)4 << 16)            // день
            | ((DWORD)12 << 11)           // час
            | ((DWORD)0 << 5)             // минута
            | ((DWORD)0 >> 1);            // секунды

}


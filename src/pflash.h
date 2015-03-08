#ifndef PFLASH_H
#define PFLASH_H
#include "stm32f10x.h"

uint32_t flash_load();
void flash_save(uint32_t v);


#endif
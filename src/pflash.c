#include "pflash.h"


uint32_t flash_load()
{
	FLASH_Unlock();
	uint32_t t = FLASH_WRProt_Pages64to67;
	FLASH_Lock();
	return t;
}

void flash_save(uint32_t v)
{
    FLASH_Status FLASHStatus, FLASHStatus2;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	while(FLASHStatus != FLASH_COMPLETE)
		FLASHStatus = FLASH_ErasePage(FLASH_WRProt_Pages64to67);
	while(FLASHStatus2 != FLASH_COMPLETE)
		FLASHStatus2 = FLASH_ProgramHalfWord(FLASH_WRProt_Pages64to67, 0xABAB);
	FLASH_Lock();
}
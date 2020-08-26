/****************************************
 *	flash.c								*
 ****************************************/

#include	<Lib/driverlib.h>
#include	<string.h>
#include	"msp.h"
#include	"define.h"
#include	"extern.h"

extern	void	dayTimeSet(void);

//void	dayTimeFlashRead(void);
//void	dayTimeFlashWrite(void);
void	indexFlashRead(void);
void	indexFlashWrite(void);
void	flashRead(void);
void	flashWrite(void);
void	flashCtr(void);

#if 0
void	dayTimeFlashRead(void)
{
	const char*	srcAdr;
	char*	dstAdr;

	srcAdr = (const char*)SYSTEM_DAYTIME_AREA1;
	dstAdr = (char*)indexDate;								//indexDate?

	memcpy(dstAdr, srcAdr, SYSTEM_INDEX_COUNT * SYSTEM_DAYTIME_BYTE);	// 10*8?

}

void	dayTimeFlashWrite(void)
{
	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR28);
	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR29);
	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR30);
	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);

	if(!FlashCtl_eraseSector(SYSTEM_DAYTIME_AREA1))
		while(1);
	if(!FlashCtl_eraseSector(SYSTEM_DAYTIME_AREA2))
		while(1);
	if(!FlashCtl_eraseSector(SYSTEM_DAYTIME_AREA3))
		while(1);
	if(!FlashCtl_eraseSector(SYSTEM_DAYTIME_AREA4))
		while(1);
	if(!FlashCtl_programMemory(indexDate,					//indexDate?
		(void*) SYSTEM_DAYTIME_AREA1, SYSTEM_INDEX_COUNT * SYSTEM_DAYTIME_BYTE))	// 10*8
			while(1);

	FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR28);
	FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR29);
	FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR30);
	FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR30);
}
#endif

void	indexFlashRead(void)
{
	const char*	srcAdr;
	char*	dstAdr;

	srcAdr = (const char*)SYSTEM_INDEX_AREA1;
	dstAdr = (char*)recordIndexBuf;

	memcpy(dstAdr, srcAdr, SYSTEM_INDEX_POINTER);

}

void	indexFlashWrite(void)
{
	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR25);
	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR26);
	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR27);

	if(!FlashCtl_eraseSector(SYSTEM_INDEX_AREA1))
		while(1);
	if(!FlashCtl_eraseSector(SYSTEM_INDEX_AREA2))
		while(1);
	if(!FlashCtl_eraseSector(SYSTEM_INDEX_AREA3))
		while(1);
	if(!FlashCtl_programMemory(recordIndexBuf,
		(void*) SYSTEM_INDEX_AREA1, SYSTEM_INDEX_POINTER))
			while(1);

	FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR25);
	FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR26);
	FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR27);
}

void	flashRead(void)
{
	const char*	srcAdr;
	char*	dstAdr;

	srcAdr = (const char*)SYSTEM_SV_AREA;
	dstAdr = (char*)svBuf;

	memcpy(dstAdr, srcAdr, SYSTEM_SV_COUNT);
//	dateTime.YearH = svBuf[0];
//	dateTime.YearL = svBuf[1];
//	dateTime.month = svBuf[2];
//	dateTime.day = svBuf[3];
//	dateTime.hour = svBuf[4];
//	dateTime.minute = svBuf[5];
//	dateTime.second = svBuf[6];

}

void	flashWrite(void)
{

	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR24);

	if(!FlashCtl_eraseSector(SYSTEM_SV_AREA))
		while(1);
	if(!FlashCtl_programMemory(svBuf,
		(void*) SYSTEM_SV_AREA, SYSTEM_SV_COUNT))
			while(1);

	FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK0,FLASH_SECTOR24);
}

void	flashCtr(void)
{
	flashRead();
	if(!((svBuf[510] == FLASHCHECK1) && (svBuf[511] == FLASHCHECK2)))
	{
		memset(svBuf, 0x00, SYSTEM_SV_COUNT);
		memcpy(svBuf, svParaDef, SV_COUNT);
		if(unitType == MASTER)
		{
			memcpy(&svBuf[7], svParaDeUnitSerialNoMaster, 20);
		} else
		{
			memcpy(&svBuf[7], svParaDeUnitSerialNoSlave, 20);
		}
		svBuf[510] = FLASHCHECK1;
		svBuf[511] = FLASHCHECK2;
//		flashWrite();
	}
	dayTimeSet();
}


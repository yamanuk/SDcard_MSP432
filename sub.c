/****************************************
 *	sub.c								*
 ****************************************/

#include	<Lib/driverlib.h>
#include	<string.h>
#include	"msp.h"
#include	"define.h"
#include	"extern.h"

extern void MAP_SPI_clearInterruptFlag(uint32_t moduleInstance, uint_fast8_t mask);

extern	DMA_ControlTable sdRXDma00Seq[RX1_TASKS];
extern	DMA_ControlTable sdRXDma01Seq[RX2_TASKS];
extern	DMA_ControlTable sdRXDma02Seq[RX3_TASKS];
extern	DMA_ControlTable sdRXDma03Seq[RX4_TASKS];
extern	DMA_ControlTable sdRXDma04Seq[RX5_TASKS];
extern	DMA_ControlTable sdRXDma05Seq[RX6_TASKS];
extern	DMA_ControlTable sdRXDma06Seq[RX7_TASKS];
extern	DMA_ControlTable sdRXDma07Seq[RX8_TASKS];
extern	DMA_ControlTable sdTXDma00Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma10Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma20Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma30Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma40Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma50Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma60Seq[SDTXTASKS];
//extern	DMA_ControlTable sdTXDma70Seq[SDTXTASKS];
extern	DMA_ControlTable usbTXDmaData00Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma00Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma02Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma04Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma08Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma16Seq[USBTXTASKS];
extern	DMA_ControlTable sramRXDma01Seq[SRAMRXTASKS01];

extern	void	indexFlashRead(void);
extern	void	indexFlashWrite(void);

extern	void	sramCSLow(void);
extern	void	sramCSHi(void);
extern	void	indexSRAMCheckWrite(void);
extern	uchar	indexSRAMCheckRead(void);
extern	void	sdAdrWrite(void);
extern	void	sdAdrRead(void);
extern	uchar	sdBlockRead(long blocknum);
extern	void	sendCrc(void);
extern	uchar	sdSendByte(uchar spiDataByte);
extern	uchar	sdGetResponse(void);
extern	void	indexSRAMPointWrite(void);
extern	void	indexSRAMPointRead(void);
extern	void	indexSRAMDayTimeWrite(void);
extern	void	indexSRAMDayTimeRead(void);
extern	void	indexSRAMWrite(void);
extern	void	indexSRAMRead(void);
extern	void	SRAMALLClear(void);
extern	uchar	sramCompTest(void);
extern	void	sramSave(void);
extern	void	sramLoad(void);
extern	void	writeOut(void);
extern	uchar	sdWRBusyChk(void);
extern	uchar	sdWRStsRead(void);
extern	void	sdReadMain(void);
//extern	uchar	sdBlockMultiWrite(long blocknum, uchar *buf, uint count);
extern	uchar	sdBlockMultiWrite(long blocknum, uint count, uchar *buf);
extern	uchar	sdBlockMultiWrite1(long blocknum, uint count);
extern	uchar	sdBlockMultiWrite2(uint count, uchar *buf);
extern	uchar	sdBlockMultiWrite3(void);
extern	void	sdWriteMain(void);
extern	void	ledD1Off(void);
extern	void	ledD1On(void);
extern	void	ledD1OnOff(void);
extern	void	ledD2Off(void);
extern	void	ledD2On(void);
extern	void	ledD2OnOff(void);
extern	void	ledD3Off(void);
extern	void	ledD3On(void);
extern	void	ledD3OnOff(void);
extern	void	ledD4Off(void);
extern	void	ledD4On(void);
extern	void	ledD4OnOff(void);
extern	void	led2Off(void);
extern	void	led2On(void);
extern	void	led2OnOff(void);
extern	void	testPort1Off(void);
extern	void	testPort1On(void);
extern	void	testPort1OnOff(void);
//extern	void	ADClkSet(void);
//extern	void	spsDisp(void);

void	delay1us(void);
void	delay5us(void);
void	delay10us(void);
void	delay30us(void);
void	delay100us(void);
void	delay1ms(void);
void	delay10ms(void);
void	delay50ms(void);
void	delay100ms(void);
void	delay1s(void);
void	resetEventTime01MS(void);
void	resetEventTime02MS(void);
void	resetEventTime05MS(void);
void	resetEventTime10MS(void);
void	resetEventTime16MS(void);
void	resetEventTime20MS(void);
void	resetEventTime40MS(void);
void	resetEventTime100MS(void);
void	resetEventTime200MS(void);
void	resetEventTime1S(void);
void	resetEventTime60S(void);
void	setEventTime01MS(void);
void	setEventTime02MS(void);
void	setEventTime05MS(void);
void	setEventTime10MS(void);
void	setEventTime16MS(void);
void	setEventTime20MS(void);
void	setEventTime40MS(void);
void	setEventTime100MS(void);
void	setEventTime200MS(void);
void	setEventTime1S(void);
void	setEventTime60S(void);
void	Timer0Reset(void);										// Command Time Reset
void	Timer0Set(ulong value);									// Command Time Set
void	Timer1Reset(void);										// 
void	Timer1Set(uint value);									// 
void	Timer2Reset(void);										// 
void	Timer2Set(ulong value);									// 
void	timeCount(void);
void	timeSyncOut(void);
void	rtcSet(void);
void	rtcGet(void);
void	dayTimeSet(void);
void	dayTimeCheck(void);
void	errCheck(void);
void	indexStartDateStore(void);
void	indexDateStore(void);
void	sdWriteAdrDet(void);
void	sramSaveCtr(void);
void	sramLoadCtr(void);
void	dataIndexCtr(void);
void	sdErrDisp(void);
//void	sdCompMain(void);
void	sdBusyCheck(void);
void	sdStatus(void);
//void	testSdWrite(void);
void	sdWriteCtr(void);
//void	sdWriteTest(void);
//void	sdRWTestMain(void);
void	commandCheck(uchar* string);

void	delay1us(void)
{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
}

void	delay5us(void)
{
	uint	cnt;

	for(cnt = DELAY01MS/200; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	delay10us(void)
{
	uint	cnt;

	for(cnt = DELAY01MS/100; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	delay30us(void)
{
	uint	cnt;

	for(cnt = DELAY01MS/33; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	delay100us(void)
{
	uint	cnt;

	for(cnt = DELAY01MS/10; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	delay1ms(void)
{
	uint	cnt;

	for(cnt = DELAY01MS; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	delay10ms(void)
{
	uint	cnt;

	for(cnt = DELAY01MS*10; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	delay50ms(void)
{
	uint	cnt;

	for(cnt = DELAY01MS*50; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	delay100ms(void)
{
	uint	cnt;

	for(cnt = DELAY01MS*100; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	delay1s(void)
{
	uint	cnt;

	for(cnt = DELAY01MS*1000; cnt > 0; cnt--)
	{
	__no_operation();										// For debugger
	__no_operation();										// For debugger
	}
}

void	resetEventTime01MS(void)
{
	eventTime01MS = CLR;
}

void	resetEventTime02MS(void)
{
	eventTime02MS = CLR;
}

void	resetEventTime05MS(void)
{
	eventTime05MS = CLR;
}

void	resetEventTime10MS(void)
{
	eventTime10MS = CLR;
}

void	resetEventTime16MS(void)
{
	eventTime16MS = CLR;
}

void	resetEventTime20MS(void)
{
	eventTime20MS = CLR;
}

void	resetEventTime40MS(void)
{
	eventTime40MS = CLR;
}

void	resetEventTime100MS(void)
{
	eventTime100MS = CLR;
}

void	resetEventTime200MS(void)
{
	eventTime200MS = CLR;
}

void	resetEventTime1S(void)
{
	eventTime1S = CLR;
}

void	resetEventTime60S(void)
{
	eventTime60S = CLR;
}

void	setEventTime01MS(void)
{
	eventTime01MS = CLR;
	time01MS = EVENT_TIME_01MS;
}

void	setEventTime02MS(void)
{
	eventTime02MS = CLR;
	time02MS = EVENT_TIME_02MS;
}

void	setEventTime05MS(void)
{
	eventTime05MS = CLR;
	time05MS = EVENT_TIME_05MS;
}

void	setEventTime10MS(void)
{
	eventTime10MS = CLR;
	time10MS = EVENT_TIME_10MS;
}

void	setEventTime16MS(void)
{
	eventTime16MS = CLR;
	time16MS = EVENT_TIME_16MS;
}

void	setEventTime20MS(void)
{
	eventTime20MS = CLR;
	time20MS = EVENT_TIME_20MS;
}

void	setEventTime40MS(void)
{
	eventTime40MS = CLR;
	time40MS = EVENT_TIME_40MS;
}

void	setEventTime100MS(void)
{
	eventTime100MS = CLR;
	time100MS = EVENT_TIME_100MS;
}

void	setEventTime200MS(void)
{
	eventTime200MS = CLR;
	time200MS = EVENT_TIME_200MS;
}

void	setEventTime1S(void)
{
	eventTime1S = CLR;
	time1S = EVENT_TIME_1S;
}

void	setEventTime60S(void)
{
	eventTime60S = CLR;
	time60S = EVENT_TIME_60S;
}

void	Timer0Reset(void)										// Command Time Reset
{
	Timer.timer0 = NULL;
}

void	Timer0Set(ulong value)									// Command Time Set
{
	Timer.timer0 = value;
}

void	Timer1Reset(void)										// 
{
	Timer.timer1 = NULL;
}

void	Timer1Set(uint value)									// 
{
	Timer.timer1 = value;
}

void	Timer2Reset(void)										// 
{
	Timer.timer2 = NULL;
}

void	Timer2Set(ulong value)									// 
{
	Timer.timer2 = value;
}

void	timeCount(void)
{
//	adcRead();
	if(Timer.timer0) Timer.timer0--;
	if(Timer.timer1) Timer.timer1--;
	if(Timer.timer2) Timer.timer2--;
	if(time01MS <= CLR) {
		eventTime01MS = SET;
	} else {
		time01MS--;
	}
	if(time02MS <= CLR) {
		eventTime02MS = SET;
	} else {
		time02MS--;
	}
	if(time05MS <= CLR) {
		eventTime05MS = SET;
	} else {
		time05MS--;
	}
	if(time10MS <= CLR) {
		eventTime10MS = SET;
	} else {
		time10MS--;
	}
	if(time16MS <= CLR) {
		eventTime16MS = SET;
	} else {
		time16MS--;
	}
	if(time20MS <= CLR) {
		eventTime20MS = SET;
	} else {
		time20MS--;
	}
	if(time40MS <= CLR) {
		eventTime40MS = SET;
	} else {
		time40MS--;
	}
	if(time100MS <= CLR) {
		eventTime100MS = SET;
	} else {
		time100MS--;
	}
	if(time200MS <= CLR) {
		eventTime200MS = SET;
	} else {
		time200MS--;
	}
	if(time1S <= CLR) {
		eventTime1S = SET;
	} else {
		time1S--;
	}
	if(time60S <= CLR) {
		eventTime60S = SET;
	} else {
		time60S--;
	}
}

void	timeSyncOut(void)
{
	if(unitType == MASTER)
	{
		if((sync1PPSFlag == ON) || (Timer.timer2 == 0))
		{
			if(syncStartFlag == ON)
			{
				P7->OUT |= BIT5;
				sync1PPSFlag = OFF;
				syncStartFlag = OFF;
				P7->OUT &= ~BIT5;
			}
		}
	}
}

void	rtcSet(void)
{
	RTCCTL0_H = RTCKEY_H ;

	RTC_C->TIM0 = (dateTime.minutes << RTC_C_TIM0_MIN_OFS) | dateTime.seconds;
	RTC_C->TIM1 = (dateTime.dayOfWeek << RTC_C_TIM1_DOW_OFS) | dateTime.hours;
	RTC_C->DATE = (dateTime.month << RTC_C_DATE_MON_OFS) | dateTime.day;
	RTC_C->YEAR = dateTime.year;

	RTCCTL0_H = 0;
}

void	rtcGet(void)
{
	dateTime.minutes = (RTC_C->TIM0 & 0xff00) >> RTC_C_TIM0_MIN_OFS;
	dateTime.seconds = RTC_C->TIM0 & 0x00ff;
	dateTime.dayOfWeek = (RTC_C->TIM1 & 0xff00) >> RTC_C_TIM1_DOW_OFS;
	dateTime.hours = RTC_C->TIM1 & 0x00ff;
	dateTime.month = (RTC_C->DATE & 0xff00) >> RTC_C_DATE_MON_OFS;
	dateTime.day = RTC_C->DATE & 0x00ff;
	dateTime.year = RTC_C->YEAR;

}

void	dayTimeSet(void)
{
	dateTime.year = (svBuf[0] << 8) + svBuf[1];
	dateTime.month = svBuf[2];
	dateTime.day = svBuf[3];
	dateTime.dayOfWeek = 0;
	dateTime.hours = svBuf[4];
	dateTime.minutes = svBuf[5];
	dateTime.seconds = svBuf[6];
	rtcSet();
	rtcGet();
}

void	dayTimeCheck(void)
{
	uint work;

	if(gpsBuffer[0] == '$')
	{
		if(((memcmp(&gpsBuffer[1], gprmcCMD, sizeof(gprmcCMD))) == 0) &&
		(gpsBuffer[17] == 'A') && (gpsBuffer[63] == '*') && (gpsBuffer[66] == CR) && (gpsBuffer[67] == LF))
		{
			work = ((gpsBuffer[57] & 0x0f) << 4) + (gpsBuffer[58] & 0x0f);
			dateTime.year = work + 0x2000;
			work = ((gpsBuffer[55] & 0x0f) << 4) + (gpsBuffer[56] & 0x0f);
			dateTime.month = work;
			work = ((gpsBuffer[53] & 0x0f) << 4) + (gpsBuffer[54] & 0x0f);
			dateTime.day = work;
			dateTime.dayOfWeek = 0;
			work = ((gpsBuffer[7] & 0x0f) << 4) + (gpsBuffer[8] & 0x0f);
			dateTime.hours = work;
			work = ((gpsBuffer[9] & 0x0f) << 4) + (gpsBuffer[10] & 0x0f);
			dateTime.minutes = work;
			work = ((gpsBuffer[11] & 0x0f) << 4) + (gpsBuffer[12] & 0x0f);
			dateTime.seconds = work;

			recPoint.lat_h = gpsBuffer[19] & 0x0f;
			recPoint.lat_l = ((gpsBuffer[20] & 0x0f) << 28) | ((gpsBuffer[21] & 0x0f) << 24) |
			((gpsBuffer[22] & 0x0f) << 20) | ((gpsBuffer[24] & 0x0f) << 16) |
			((gpsBuffer[25] & 0x0f) << 12) | ((gpsBuffer[26] & 0x0f) << 8) |
			((gpsBuffer[27] & 0x0f) << 4) | (gpsBuffer[28] & 0x0f);

			recPoint.lng_h = ((gpsBuffer[32] & 0x0f) << 4) | (gpsBuffer[33] & 0x0f);
			recPoint.lng_l = ((gpsBuffer[34] & 0x0f) << 28) | ((gpsBuffer[35] & 0x0f) << 24) |
			((gpsBuffer[36] & 0x0f) << 20) | ((gpsBuffer[38] & 0x0f) << 16) |
			((gpsBuffer[39] & 0x0f) << 12) | ((gpsBuffer[40] & 0x0f) << 8) |
			((gpsBuffer[41] & 0x0f) << 4) | (gpsBuffer[42] & 0x0f);
			rtcSet();
			rtcGet();
			timeGet =ON;
			gpsCtr = 1;
		} else {
			if(((memcmp(&gpsBuffer[1], gpggaCMD, sizeof(gpggaCMD))) == 0) &&
			(gpsBuffer[44] != '0') && (gpsBuffer[70] == '*') && (gpsBuffer[73] == CR) && (gpsBuffer[74] == LF))
			{
				recPoint.alt = ((gpsBuffer[54] & 0x0f) << 12) | ((gpsBuffer[55] & 0x0f) << 8) | ((gpsBuffer[56] & 0x0f) << 4) | (gpsBuffer[58] & 0x0f);
				altGet = ON;
				gpsCtr = 0;
			}
		}
	}
}

void	errCheck(void)
{
	if(adErrStatus == AD_HEAD_ERR)
	{
		adErrRetry++;
		if(adErrRetry >= AD_RETRY_ERR_COUNT)
		{
			ledD3OnOff();
//			initADCSet();
			adErrRetry = 0;
		}
	} else if(adErrStatus == NO_ERR)
	{
		adErrRetry = 0;
	}
	if(sdErrStatus != NO_ERR)
	{
		__no_operation();
	}
}

void	indexStartDateStore(void)
{
	indexStoreDate[0] = (dateTime.year & 0xff00) >> 8;
	indexStoreDate[1] = dateTime.year & 0x00ff;
	indexStoreDate[2] = dateTime.month;
	indexStoreDate[3] = dateTime.day;
	indexStoreDate[4] = dateTime.hours;
	indexStoreDate[5] = dateTime.minutes;
	indexAdrBuf[indexDatePointer] = sdMainCount;
}

void	indexDateStore(void)
{
	indexDate[indexDatePointer][0] = indexStoreDate[0];
	indexDate[indexDatePointer][1] = indexStoreDate[1];
	indexDate[indexDatePointer][2] = indexStoreDate[2];
	indexDate[indexDatePointer][3] = indexStoreDate[3];
	indexDate[indexDatePointer][4] = indexStoreDate[4];
	indexDate[indexDatePointer][5] = indexStoreDate[5];
	indexDate[indexDatePointer][6] = dateTime.hours;
	indexDate[indexDatePointer][7] = dateTime.minutes;
}

void	sdWriteAdrDet(void)
{
	
	for(indexCount = 0; indexCount < SYSTEM_INDEX_COUNT; indexCount++)
	{
		if((recordIndexBuf[indexCount * 5] == 0) && (recordIndexBuf[indexCount * 5 + 1] == 0) && (recordIndexBuf[indexCount * 5 + 2] == 0) &&
		(recordIndexBuf[indexCount * 5 + 3] == 0) && ((recordIndexBuf[indexCount * 5 + 4] == 0) || (recordIndexBuf[indexCount * 5 + 4] == 1)))
		{
			if(indexCount > 0)
			{
				indexCount--;
			} else
			{
				indexCount -= 1;
				sdMainCount = 0;
				return;
			}
			break;
		}
	}
	sdMainCount = ((((((((recordIndexBuf[indexCount * 5] * 0x100) + recordIndexBuf[indexCount * 5 + 1]) * 0x100) + recordIndexBuf[indexCount * 5 + 2]) * 0x100) + 
	recordIndexBuf[indexCount * 5 + 3]) * 0x100) + recordIndexBuf[indexCount * 5 + 4]);
}

void	sramLoadCtr(void)
{
	if(sramStart == ON)
	{
		if(sramSaveStep ==STEP0)
		{
			sramLoadEndAdr = sramSaveCount;
			if(sramLoadStartAdr > sramLoadEndAdr)
			{
				sramLoadEndAdr += SRAM_MAX_ADRESS;
			}
			if((sramLoadEndAdr - sramLoadStartAdr) >= SRAM_LOAD_LENGTH)
			{
				if(sramErrStatus == NO_ERR)
				{
					switch(sramLoadStep)
					{
					case STEP0:
						sramLoad();
						sramLoadStep++;
						break;
					case STEP1:
						if(readSRAMDMAFlag == ON)
						{
							readSRAMDMAFlag = OFF;
							switch(readSRAMDMAStatus)
							{
								case SRAMREADDMASTATUS01:
									MAP_DMA_setChannelScatterGather(SRAMDMACHSPI,SRAMRXTASKS01,(void*)&sramRXDma01Seq[0],0);
									MAP_DMA_clearInterruptFlag(SRAMDMACHSPI & 0x0F);
									MAP_DMA_enableInterrupt(INT_DMA_INT3);
									MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
									MAP_DMA_enableChannel(SRAMDMACH);
									EUSCI_A1_SPI->IFG |= EUSCI_A_IFG_RXIFG;
									readSRAMDMAStatus++;
									return;
								case SRAMREADDMASTATUS257:
//									memcpy(&sramReadBuf[(readSRAMDMAStatus -2) * SRAM_READ_LENGTH], &sramWorkReadBuf[0], SRAM_READ_LENGTH);
									readSRAMDMAStatus = SRAMREADDMASTATUS00;
									sramCSHi();
//									writeOut();
									return;
								default:
//									memcpy(&sramReadBuf[(readSRAMDMAStatus -2) * SRAM_READ_LENGTH], &sramWorkReadBuf[0], SRAM_READ_LENGTH);
									MAP_DMA_setChannelScatterGather(SRAMDMACHSPI,SRAMRXTASKS01,(void*)&sramRXDma01Seq[0],0);
									break;
							}
//							MAP_SPI_clearInterruptFlag(EUSCI_A1_BASE,EUSCI_A_SPI_RECEIVE_INTERRUPT);
							MAP_DMA_clearInterruptFlag(SRAMDMACHSPI & 0x0F);
							MAP_DMA_enableInterrupt(INT_DMA_INT3);
							MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
							MAP_DMA_enableChannel(SRAMDMACH);
							EUSCI_A1_SPI->IFG |= EUSCI_A_IFG_RXIFG;
							readSRAMDMAStatus++;
//							writeOut();
						}
						if(readSRAMDMAStatus == 0)
						{
							sramLoadOK = ON;
							sramLoadCount += SRAM_LOAD_LENGTH;
							if(sramLoadCount >= SRAM_MAX_ADRESS)					// 0x80000
							{
								sramLoadCount -= SRAM_MAX_ADRESS;
							}
							sramLoadStartAdr += SRAM_LOAD_LENGTH;
							if(sramLoadStartAdr >= SRAM_MAX_ADRESS)
							{
								sramLoadStartAdr -= SRAM_MAX_ADRESS;
							}
							sramLoadStep = STEP0;
						}
					}
				}
			}
		}
	}
}

void	sramSaveCtr(void)
{
	if(sramStart == ON)
	{
		if(sramLoadStep ==0)
		{
			sramSaveEndAdr = adBufAdr;
			if(sramSaveStartAdr > sramSaveEndAdr)
			{
				sramSaveEndAdr += AD_BUF_LENGTH;
			}
			if((sramSaveEndAdr - sramSaveStartAdr) >= SRAM_SAVE_LENGTH)
			{
				if(sramErrStatus == NO_ERR)
				{
					switch(sramSaveStep)
					{
					case STEP0:
ledD4On();
						memcpy(&sramWriteBuf, &adBuf[sramSaveStartAdr], SRAM_SAVE_LENGTH);
						sramSave();
						sramSaveStep++;
						break;
					case STEP1:
						if(writeSRAMDMAStatus == 0)
						{
ledD4Off();
							sramSaveCount += SRAM_SAVE_LENGTH;
							if(sramSaveCount >= SRAM_MAX_ADRESS)					// 0x80000
							{
								sramSaveCount -= SRAM_MAX_ADRESS;
							}
							sramSaveStartAdr += SRAM_SAVE_LENGTH;
							if(sramSaveStartAdr >= AD_BUF_LENGTH)
							{
								sramSaveStartAdr -= AD_BUF_LENGTH;
							}
							sramSaveStep = STEP0;
						}
					}
				}
			}
		}
	}
}

void	dataIndexCtr(void)
{
//	sdAdrWrite();
	if(indexSRAMCheckRead() == NG)
	{
		SRAMALLClear();
		indexSRAMCheckWrite();
//		indexFlashRead();
		indexFlashRead();
//		sdWriteAdrDet();
	} else
	{
//		SRAMALLClear();
//		SRAMALLRead();
		sdAdrRead();
//		sdWriteAdrDet();
		indexCount++;
		sdAdrWrite();
//		indexFlashWrite();
//		indexFlashRead();
		indexSRAMPointRead();
		indexSRAMDayTimeRead();
		indexSRAMRead();
	}
}

void	sdErrDisp(void)
{
	if(sdErrStatus != NO_ERR)
	{
		ledD3OnOff();
//		sdErrStatus = NO_ERR;
	}
}

#if 0
void	sdCompMain(void)
{
	uint	cnt;

	for(cnt = 0; cnt < SD_COMP_SECTOR; cnt++)
	{
		testCount = (ulong)(sdWriteBuf[SD_BLOCK_SIZE * cnt + 0] << 24);
		testCount |= (ulong)(sdWriteBuf[SD_BLOCK_SIZE * cnt + 1] << 16);
		testCount |= (ulong)(sdWriteBuf[SD_BLOCK_SIZE * cnt + 2] << 8);
		testCount |= (ulong)sdWriteBuf[SD_BLOCK_SIZE * cnt + 3];
		if(sdCount + cnt != testCount)
		{
			sdErrStatus = SD_COMP_ERR;
			ledD3OnOff();
		}
	}
}
#endif

void	sdBusyCheck(void)
{
	uchar	rep;

	if(writeSDBusyChk == ON)
	{
		SD_NSS_Low;												// Set CS low, i.e. select card
		rep = sdWRBusyChk();
		SD_NSS_High;											// Set CS high, i.e. deselect card
		if(rep != 0)
		{
			writeSDBusyChk = OFF;
		}
	}
	if(readSDBusyChk == ON)
	{
		readSDBusyChk = OFF;
	}
}

#if 0
void	sdStatus(void)
{
	uchar	rep;

	if(writeSDStatusChk == ON)
	{
		switch(sdStatusStep)
		{
			case STEP0:
				sendCrc();
				rep =sdWRStsRead();
				if(rep != SD_WRITE_SUCCESS)
				{
					sdErrStatus = SD_WRIT_ERR;
				}
				sdStatusStep +=1;
				break;
			case STEP1:
				rep = sdWRBusyChk();
				if(rep != 0)
				{
					sdStatusStep +=1;
				}
				break;
			case STEP2:
				switch(writeSDDMAStatus)
				{
					case	SDWRITEDMASTATUS01:
						sdSendByte(0xFC);	// Send Start Block Token
						MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma10Seq[0],0);
						break;
					case	SDWRITEDMASTATUS02:
						sdSendByte(0xFC);	// Send Start Block Token
						MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma20Seq[0],0);
						break;
					case	SDWRITEDMASTATUS03:
						sdSendByte(0xFC);	// Send Start Block Token
						MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma30Seq[0],0);
						break;
					case	SDWRITEDMASTATUS04:
						sdSendByte(0xFC);	// Send Start Block Token
						MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma40Seq[0],0);
						break;
					case	SDWRITEDMASTATUS05:
						sdSendByte(0xFC);	// Send Start Block Token
						MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma50Seq[0],0);
						break;
					case	SDWRITEDMASTATUS06:
//						sendCrc();
//						rep =sdWRStsRead();
//						if(rep != SD_WRITE_SUCCESS)
//						{
//							sdErrStatus = SD_WRIT_ERR;
//						}
//						rep = 0;
//						while(rep == 0)
//						{
//							rep = sdWRBusyChk();
//						}
//						sdSendByte(0xFC);	// Send Start Block Token
//						MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma60Seq[0],0);
//						break;
//					case	SDWRITEDMASTATUS07:
						sdSendByte(0xFD);
						rep = sdGetResponse();
						if(rep != 0x00)
						{
							sdErrStatus = SD_WRIT_ERR;
						}
						writeSDExe = OFF;
						writeSDBusyChk = ON;
						SD_NSS_High;												// Set CS high, i.e. deselect card
						sdSendByte(0xFF);
						writeSDDMAStatus = SDWRITEDMASTATUS00;
						writeSDStatusChk = OFF;
						sdStatusStep =0;
						return;
				}
				MAP_DMA_clearInterruptFlag(SDDMACHSPI & 0x0F);
				MAP_DMA_enableInterrupt(INT_DMA_INT1);
				MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
				MAP_DMA_enableChannel(7);
				EUSCI_B3_SPI->IFG |= EUSCI_B_IFG_RXIFG;
				writeSDDMAStatus++;
				writeSDStatusChk = OFF;
				sdStatusStep =0;
		}
	}
}
#endif

uchar	compReadBuf(long blocknum)
{
	long	work;

//	if((sdReadBuf[3] != (uchar)(readSDCount & 0x000000ff)) ||
//	(sdReadBuf[2] != (uchar)((readSDCount & 0x0000ff00) >> 8)) ||
//	(sdReadBuf[1] != (uchar)((readSDCount & 0x00ff0000) >> 16)) ||
//	(sdReadBuf[0] != (uchar)((readSDCount & 0xff000000) >> 24)))
	work = blocknum;

	if((work % 3) == 0)
	{
		work = work / 3 * 0x40;
		if(
		(sdReadBuf[3*0 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sdReadBuf[3*0 + 2] != (uchar)(work & 0x000000ff)) ||
		(sdReadBuf[3*1 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sdReadBuf[3*1 + 2] != (uchar)(work & 0x000000ff)) ||
		(sdReadBuf[3*2 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sdReadBuf[3*2 + 2] != (uchar)(work & 0x000000ff)) ||
		(sdReadBuf[3*3 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sdReadBuf[3*3 + 2] != (uchar)(work & 0x000000ff)) ||
		(sdReadBuf[3*4 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sdReadBuf[3*4 + 2] != (uchar)(work & 0x000000ff)) ||
		(sdReadBuf[3*5 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sdReadBuf[3*5 + 2] != (uchar)(work & 0x000000ff)) ||
		(sdReadBuf[3*6 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sdReadBuf[3*6 + 2] != (uchar)(work & 0x000000ff)) ||
		(sdReadBuf[3*7 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sdReadBuf[3*7 + 2] != (uchar)(work & 0x000000ff)))
		{
			return 0xff;
		}
	}
	return 0x00;
}

void	sdWriteCtr(void)
{
	uchar	rep;
//	uchar	buf;
//	uint	i;

#if 0
					memset(sdBuf, (uchar)readSDCount, SD_BLOCK_SIZE);
					rep = sdBlockMultiWrite(readSDCount, SD_WRITE_SECTOR, (uchar*) sdBuf);
					if(rep != 0)
					{
						sdErrStatus = SD_WRIT_ERR;
					}
					rep = sdBlockRead(readSDCount);
					if(rep != 0)
					{
						sdErrStatus = SD_READ_ERR;
					}
					if(memcmp(sdBuf, sdReadBuf, SD_BLOCK_SIZE) != 0) 
					{
						sdErrStatus = SD_COMP_ERR;
					}
					readSDCount += SD_WRITE_SECTOR;
					if(readSDCount >= sdMaxSectors)
					{
						readSDCount = 0;
					}
#else
	if(SDCard.Err_Flag == OK)
	{
		if(sdStart == ON)
		{
#if 1
			ledD2On();
			switch(sdStep)
			{
				case	STEP0:
					if(sdErrStatus == NO_ERR)
					{
//						rep = sdBlockMultiWrite(readSDCount, SD_WRITE_SECTOR, (uchar*) sdBuf);
//ledD2On();
						rep = sdBlockMultiWrite1(readSDCount, TESTWSECTORT);
						if(rep != 0)
						{
							sdErrStatus = SD_WRIT_ERR;
						}
//ledD2Off();
					}
					sdStep++;
					break;
				case	STEP1:
					if(sdErrStatus == NO_ERR)
					{
						rep = sdBlockMultiWrite2(TESTWSECTORT, (uchar*) sdBuf);
						if(rep != 0)
						{
							sdErrStatus = SD_WRIT_ERR;
						}
					}
					sdStep++;
					break;
				case	STEP2:
					if(sdErrStatus == NO_ERR)
					{
//ledD2On();
						rep = sdBlockMultiWrite3();
						if(rep != 0)
						{
							sdErrStatus = SD_WRIT_ERR;
						}
//ledD2Off();
						readSDCount += SD_WRITE_SECTOR;
						if(readSDCount >= sdMaxSectors)
						{
							readSDCount = 0;
						}
					}
					sdStep = 0;
					break;
			}
#else
			if(sramLoadOK == ON)
			{
				memcpy(&sdBuf[sdBufAdr], sramReadBuf, SRAM_LOAD_LENGTH);
				sdBufAdr += SRAM_LOAD_LENGTH;
				if(sdBufAdr >= SD_WRITE_LENGTH)
				{
					sdBufAdr = 0;
					switch(sdStep)
					{
						case	STEP0:
							if(sdErrStatus == NO_ERR)
							{
//								rep = sdBlockMultiWrite(readSDCount, SD_WRITE_SECTOR, (uchar*) sdBuf);
								rep = sdBlockMultiWrite1(readSDCount, SD_WRITE_SECTOR);
								if(rep != 0)
								{
									sdErrStatus = SD_WRIT_ERR;
								}
							}
							sdStep++;
							break;
						case	STEP1:
							if(sdErrStatus == NO_ERR)
							{
								if( sdSubStep < SD_WRITE_SECTOR)
								{
									sdSubStep++;
									rep = sdBlockMultiWrite2(SD_WRITE_SECTOR, (uchar*) sdBuf);
									if(rep != 0)
									{
										sdErrStatus = SD_WRIT_ERR;
									}
								} else
								{
									sdSubStep = 0;
									sdStep++;
								}
							}
							sdStep++;
							break;
						case	STEP2:
							if(sdErrStatus == NO_ERR)
							{
								rep = sdBlockMultiWrite3();
								if(rep != 0)
								{
									sdErrStatus = SD_WRIT_ERR;
								}
							}
							sdStep++;
							break;
						case	STEP3:
							readSDCount += SD_WRITE_SECTOR;
							if(readSDCount >= sdMaxSectors)
							{
								readSDCount = 0;
							}
							sdStep = 0;
							break;
					}
				}
				sramLoadOK = OFF;
			}
#endif
ledD2Off();
		}
	}
#endif
}

#if 0
void	sdWriteTest(void)
{
	uchar	rep;

	if((writeSDBusyChk == OFF) && (writeSDStatusChk == OFF))
	{
		memcpy(sdWriteBuf, adBuf, SD_WRITE_LENGTH);
		sdMainCount += SD_WRITE_SECTOR;
		if(sdMainCount >= sdMaxSectors)
		{
			sdMainCount = 1;
		}
		SD_NSS_Low;	// Set CS low, i.e. select card
		rep = sdBlockMultiWrite(sdMainCount, &sdWriteBuf[0], SD_WRITE_SECTOR);
		if(rep != 0)
		{
			sdErrStatus = SD_WRIT_ERR;
		}
	}
	sdStatus();
	sdBusyCheck();
}
#endif

#if 0
void	commandCheck(uchar* string)
{
	uint	i;

	if(((string[0] & 0xdf)== stopCommand[0]) && ((string[1] & 0xdf) == stopCommand[1]) &&
		((string[2] & 0xdf) == stopCommand[2]) && ((string[3] & 0xdf) == stopCommand[3]))
	{
		rtdEnable = OFF;
	} else if(((string[0] & 0xdf)== startCommand[0]) && ((string[1] & 0xdf) == startCommand[1]) &&
		((string[2] & 0xdf) == startCommand[2]) && ((string[3] & 0xdf) == startCommand[3])
		 && ((string[4] & 0xdf) == startCommand[4]))
	{
		rtdEnable = ON;
	}
	if(((string[0] & 0xdf)== sps100000Command[0]) && ((string[1] & 0xdf) == sps100000Command[1]) &&
		((string[2] & 0xdf) == sps100000Command[2]) && (string[3] == sps100000Command[3]) &&
		(string[4] == sps100000Command[4]) && (string[5] == sps100000Command[5]) &&
		(string[6] == sps100000Command[6]) && (string[7] == sps100000Command[7]) &&
		(string[8] == sps100000Command[8]))
	{
		rtdSPS = SPS100000FLAG;
//		ADClkSet();
		spsDisp();
	} else if(((string[0] & 0xdf)== sps10000Command[0]) && ((string[1] & 0xdf) == sps10000Command[1]) &&
		((string[2] & 0xdf) == sps10000Command[2]) && (string[3] == sps10000Command[3]) &&
		(string[4] == sps10000Command[4]) && (string[5] == sps10000Command[5]) &&
		(string[6] == sps10000Command[6]) && (string[7] == sps10000Command[7]))
	{
		rtdSPS = SPS10000FLAG;
//		ADClkSet();
		spsDisp();
	} else if(((string[0] & 0xdf)== sps1000Command[0]) && ((string[1] & 0xdf) == sps1000Command[1]) &&
		((string[2] & 0xdf) == sps1000Command[2]) && (string[3] == sps1000Command[3]) &&
		(string[4] == sps1000Command[4]) && (string[5] == sps1000Command[5]) &&
		(string[6] == sps1000Command[6]))
	{
		rtdSPS = SPS1000FLAG;
//		ADClkSet();
		spsDisp();
	} else if(((string[0] & 0xdf)== spsDispCommand[0]) && ((string[1] & 0xdf) == spsDispCommand[1]) &&
		((string[2] & 0xdf) == spsDispCommand[2]))
	{
		spsDisp();
	} else if(string[0] == ESC)
	{
		if(rtdEnable == ON)
		{
			rtdEnable = OFF;
		} else
		{
			rtdEnable = ON;
		}
	}
	// Clear the string in preparation for the next one
	for (i = 0; i < MAX_STR_LENGTH; i++){
		string[i] = 0x00;
	}
}
#endif


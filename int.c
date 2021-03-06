/****************************************
 *	int.c								*
 ****************************************/

#include	<Lib/driverlib.h>
#include	<string.h>
#include	"msp.h"
#include	"define.h"
#include	"extern.h"

extern void MAP_SPI_clearInterruptFlag(uint32_t moduleInstance, uint_fast8_t mask);

extern	DMA_ControlTable sdRXDma00Seq[RX1_TASKS];
//extern	DMA_ControlTable sdRXDma01Seq[RX2_TASKS];
//extern	DMA_ControlTable sdRXDma02Seq[RX3_TASKS];
//extern	DMA_ControlTable sdRXDma03Seq[RX4_TASKS];
//extern	DMA_ControlTable sdRXDma04Seq[RX5_TASKS];
//extern	DMA_ControlTable sdRXDma05Seq[RX6_TASKS];
//extern	DMA_ControlTable sdRXDma06Seq[RX7_TASKS];
//extern	DMA_ControlTable sdRXDma07Seq[RX8_TASKS];
extern	DMA_ControlTable sdTXDma00Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma10Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma20Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma30Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma40Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma50Seq[SDTXTASKS];
//extern	DMA_ControlTable sdTXDma60Seq[SDTXTASKS];
//extern	DMA_ControlTable sdTXDma70Seq[SDTXTASKS];
extern	DMA_ControlTable usbTXDmaData00Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma00Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma02Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma04Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma08Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma16Seq[USBTXTASKS];
extern	DMA_ControlTable sramRXDma01Seq[SRAMRXTASKS01];

extern	void	sramCSLow(void);
extern	void	sramCSHi(void);
extern	char	ADPCM_Encoder(signed int Input);
extern	signed	int	ADPCM_Decoder(char	temp);
extern	void	testAdpcm(void);
extern	void	sdAdrWrite(void);
extern	void	adcHeaderChk(void);
extern	void	adcRead(void);
extern	void	sramSaveCtr(void);
extern	void	sdCheck(void);
extern	void	sendCrc(void);
extern	uchar	sdSendByte(uchar spiDataByte);
extern	void	sdSendCmd(const uchar cmd, ulong data, const char crc);
extern	uchar	sdGetResponseCMD(void);
extern	uchar	sdGetResponse(void);
extern	uchar	sdCheckBusy(void);
extern	uchar	sdWRBusyChk(void);
extern	uchar	sdWRStsRead(void);
//extern	void	sdCompMain(void);
extern	uchar	sdBlockRead(long blocknum);
extern	uchar	sdBlockMultiWrite(long blocknum, uchar *buf, uint count);
extern	uchar	sdBlockWrite(long blocknum, uchar *buf);
extern	void	usbDMAWrite(void);
extern	void	adcDMARead(void);
extern	void	Timer0Reset(void);
extern	void	Timer0Set(ulong value);
extern	void	Timer1Set(uint value);
extern	void	Timer2Set(ulong value);
extern	void	timeCount(void);
extern	void	rtcGet(void);
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
extern	void	error(void);

/* Completion interrupt for DMA */
void	dmaWriteProcess(void);


/* Completion interrupt for DMA */

void DMA_INT1_IRQHandler(void)
{
	if(readSDExe == ON)
	{
		MAP_DMA_disableChannel(SDDMACH);
		MAP_DMA_disableInterrupt(INT_DMA_INT1);
		MAP_Interrupt_disableInterrupt(INT_DMA_INT1);
		MAP_SPI_clearInterruptFlag(EUSCI_B3_BASE,EUSCI_B_IFG_RXIFG);
		MAP_DMA_clearInterruptFlag(SDDMACH);
		switch(readSDDMAStatus)
		{
			case	SDREADDMASTATUS01:
				memcpy(&sdWorkReadBuf[0], &sdReadBuf[0], SD_READ_SIZE);
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&sdRXDma00Seq[0],0);
				break;
			case	SDREADDMASTATUS02:
				memcpy(&sdWorkReadBuf[64], &sdReadBuf[0], SD_READ_SIZE);
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&sdRXDma00Seq[0],0);
				break;
			case	SDREADDMASTATUS03:
				memcpy(&sdWorkReadBuf[128], &sdReadBuf[0], SD_READ_SIZE);
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&sdRXDma00Seq[0],0);
				break;
			case	SDREADDMASTATUS04:
				memcpy(&sdWorkReadBuf[192], &sdReadBuf[0], SD_READ_SIZE);
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&sdRXDma00Seq[0],0);
				break;
			case	SDREADDMASTATUS05:
				memcpy(&sdWorkReadBuf[256], &sdReadBuf[0], SD_READ_SIZE);
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&sdRXDma00Seq[0],0);
				break;
			case	SDREADDMASTATUS06:
				memcpy(&sdWorkReadBuf[320], &sdReadBuf[0], SD_READ_SIZE);
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&sdRXDma00Seq[0],0);
				break;
			case	SDREADDMASTATUS07:
				memcpy(&sdWorkReadBuf[384], &sdReadBuf[0], SD_READ_SIZE);
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&sdRXDma00Seq[0],0);
				break;
			case	SDREADDMASTATUS08:
				memcpy(&sdWorkReadBuf[448], &sdReadBuf[0], SD_READ_SIZE);
				sendCrc();
//				sdSendCmd(12,0x00000000,0x01);
//				rep = sdGetResponseCMD();
//				if(rep == ERROR)
//				{
//					sdErrStatus = SD_READ_ERR;
//				}
				sdSendByte(0xFF);
				SD_NSS_High;
				readSDDMAStatus = SDREADDMASTATUS00;
				return;
		}
		MAP_DMA_clearInterruptFlag(SDDMACHSPI & 0x0F);
		MAP_DMA_enableInterrupt(INT_DMA_INT1);
		MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
		MAP_DMA_enableChannel(SDDMACH);
		EUSCI_B3_SPI->IFG |= EUSCI_B_IFG_RXIFG;
		readSDDMAStatus++;
	}
	if(writeSDExe == ON)
	{
		MAP_DMA_disableChannel(SDDMACH);
		MAP_DMA_disableInterrupt(INT_DMA_INT1);
		MAP_Interrupt_disableInterrupt(INT_DMA_INT1);
		MAP_DMA_clearInterruptFlag(7);
#if 1
//		writeSDStatusChk = ON;
		writeSDExe = OFF;
#else
		switch(writeSDDMAStatus)
		{
			case	SDWRITEDMASTATUS01:
				sendCrc();
				rep =sdWRStsRead();
				if(rep != SD_WRITE_SUCCESS)
				{
					sdErrStatus = SD_WRIT_ERR;
				}
				rep = 0;
				while(rep == 0)
				{
					rep = sdWRBusyChk();
				}
				sdSendByte(0xFC);	// Send Start Block Token
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma10Seq[0],0);
				break;
			case	SDWRITEDMASTATUS02:
				sendCrc();
				rep =sdWRStsRead();
				if(rep != SD_WRITE_SUCCESS)
				{
					sdErrStatus = SD_WRIT_ERR;
				}
				rep = 0;
				while(rep == 0)
				{
					rep = sdWRBusyChk();
				}
				sdSendByte(0xFC);	// Send Start Block Token
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma20Seq[0],0);
				break;
			case	SDWRITEDMASTATUS03:
				sendCrc();
				rep =sdWRStsRead();
				if(rep != SD_WRITE_SUCCESS)
				{
					sdErrStatus = SD_WRIT_ERR;
				}
				rep = 0;
				while(rep == 0)
				{
					rep = sdWRBusyChk();
				}
				sdSendByte(0xFC);	// Send Start Block Token
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma30Seq[0],0);
				break;
			case	SDWRITEDMASTATUS04:
				sendCrc();
				rep =sdWRStsRead();
				if(rep != SD_WRITE_SUCCESS)
				{
					sdErrStatus = SD_WRIT_ERR;
				}
				rep = 0;
				while(rep == 0)
				{
					rep = sdWRBusyChk();
				}
				sdSendByte(0xFC);	// Send Start Block Token
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma40Seq[0],0);
				break;
			case	SDWRITEDMASTATUS05:
				sendCrc();
				rep =sdWRStsRead();
				if(rep != SD_WRITE_SUCCESS)
				{
					sdErrStatus = SD_WRIT_ERR;
				}
				rep = 0;
				while(rep == 0)
				{
					rep = sdWRBusyChk();
				}
				sdSendByte(0xFC);	// Send Start Block Token
				MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma50Seq[0],0);
				break;
			case	SDWRITEDMASTATUS06:
//				sendCrc();
//				rep =sdWRStsRead();
//				if(rep != SD_WRITE_SUCCESS)
//				{
//					sdErrStatus = SD_WRIT_ERR;
//				}
//				rep = 0;
//				while(rep == 0)
//				{
//					rep = sdWRBusyChk();
//				}
//				sdSendByte(0xFC);	// Send Start Block Token
//				MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma60Seq[0],0);
//				break;
//			case	SDWRITEDMASTATUS07:
				sendCrc();
				rep =sdWRStsRead();
				if(rep != SD_WRITE_SUCCESS)
				{
					sdErrStatus = SD_WRIT_ERR;
				}
				rep = 0;
				while(rep == 0)
				{
					rep = sdWRBusyChk();
				}
				sdSendByte(0xFD);
				rep = sdGetResponse();
				if(rep != 0x00)
				{
					sdErrStatus = SD_WRIT_ERR;
				}
//			rep = 0x00;
//			while(rep != 0xff)
//			{
//				rep = sdWRBusyChk();
//			}
				writeSDExe = OFF;
				writeSDBusyChk = ON;
				SD_NSS_High;												// Set CS high, i.e. deselect card
				sdSendByte(0xFF);
				writeSDDMAStatus = SDWRITEDMASTATUS00;
				return;
		}
		MAP_DMA_clearInterruptFlag(SDDMACHSPI & 0x0F);
		MAP_DMA_enableInterrupt(INT_DMA_INT1);
		MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
		MAP_DMA_enableChannel(7);
		EUSCI_B3_SPI->IFG |= EUSCI_B_IFG_RXIFG;
		writeSDDMAStatus++;
#endif
	}
}

#if 1
void	DMA_INT2_IRQHandler(void)
{
	MAP_DMA_disableChannel(ADDMACH);
	MAP_DMA_disableInterrupt(INT_DMA_INT2);
	MAP_Interrupt_disableInterrupt(INT_DMA_INT2);
	MAP_SPI_clearInterruptFlag(EUSCI_B2_BASE,EUSCI_B_IFG_RXIFG);
	MAP_DMA_clearInterruptFlag(ADDMACH);

	P6OUT |= BIT2;												//

#if 0
	memcpy(&adBuf[adBufAdr], adWorkBuf, AD_DATA_COUNT);
#else
	memset(&adBuf[adBufAdr], 0, AD_DATA_COUNT);
//	adBuf[adBufAdr + 0] = (testData & 0xff0000) >> 16;
	adBuf[adBufAdr + 1] = (testData & 0x00ff00) >> 8;
	adBuf[adBufAdr + 2] = (testData & 0x0000ff);
	adBuf[adBufAdr + 4] = (testData & 0x00ff00) >> 8;
	adBuf[adBufAdr + 5] = (testData & 0x0000ff);
	adBuf[adBufAdr + 7] = (testData & 0x00ff00) >> 8;
	adBuf[adBufAdr + 8] = (testData & 0x0000ff);
	adBuf[adBufAdr + 10] = (testData & 0x00ff00) >> 8;
	adBuf[adBufAdr + 11] = (testData & 0x0000ff);
	adBuf[adBufAdr + 13] = (testData & 0x00ff00) >> 8;
	adBuf[adBufAdr + 14] = (testData & 0x0000ff);
	adBuf[adBufAdr + 16] = (testData & 0x00ff00) >> 8;
	adBuf[adBufAdr + 17] = (testData & 0x0000ff);
	adBuf[adBufAdr + 19] = (testData & 0x00ff00) >> 8;
	adBuf[adBufAdr + 20] = (testData & 0x0000ff);
	adBuf[adBufAdr + 22] = (testData & 0x00ff00) >> 8;
	adBuf[adBufAdr + 23] = (testData & 0x0000ff);
	testData ++;
	if(testData > 0x007fff)
	{
		testData = 0;
	}
#endif
	adBufAdr += AD_DATA_COUNT;
	if(adBufAdr >= AD_BUF_LENGTH)
	{
		adBufAdr = 0;
	}
	adcHeaderChk();
}
#else
void	DMA_INT2_IRQHandler(void)
{
	MAP_DMA_disableChannel(ADDMACH);
	MAP_DMA_disableInterrupt(INT_DMA_INT2);
	MAP_Interrupt_disableInterrupt(INT_DMA_INT2);
	MAP_SPI_clearInterruptFlag(EUSCI_B2_BASE,EUSCI_B_IFG_RXIFG);
	MAP_DMA_clearInterruptFlag(ADDMACH);

	P6OUT |= BIT2;												//

	if(polarityFlag == 0)
	{
		memset(&adBuf[adBufAdr], 0, AD_DATA_COUNT);
//		adBuf[adBufAdr + 0] = (testData & 0xff0000) >> 16;
		adBuf[adBufAdr + 1] = (testData & 0x00ff00) >> 8;
		adBuf[adBufAdr + 2] = (testData & 0x0000ff);
		adBuf[adBufAdr + 4] = (testData & 0x00ff00) >> 8;
		adBuf[adBufAdr + 5] = (testData & 0x0000ff);
		adBuf[adBufAdr + 7] = (testData & 0x00ff00) >> 8;
		adBuf[adBufAdr + 8] = (testData & 0x0000ff);
		adBuf[adBufAdr + 10] = (testData & 0x00ff00) >> 8;
		adBuf[adBufAdr + 11] = (testData & 0x0000ff);
		adBuf[adBufAdr + 13] = (testData & 0x00ff00) >> 8;
		adBuf[adBufAdr + 14] = (testData & 0x0000ff);
		adBuf[adBufAdr + 16] = (testData & 0x00ff00) >> 8;
		adBuf[adBufAdr + 17] = (testData & 0x0000ff);
		adBuf[adBufAdr + 19] = (testData & 0x00ff00) >> 8;
		adBuf[adBufAdr + 20] = (testData & 0x0000ff);
		adBuf[adBufAdr + 22] = (testData & 0x00ff00) >> 8;
		adBuf[adBufAdr + 23] = (testData & 0x0000ff);
		testData ++;
		if(testData > 0x007fff)
		{
			testData = 0;
		}
		adBufAdr += AD_DATA_COUNT;
		if(adBufAdr >= AD_BUF_LENGTH)
		{
			adBufAdr = 0;
		}
		polarityFlag ++;
	} else
	{
		polarityFlag ++;
		if(polarityFlag == 4)
		{
			polarityFlag = 0;
		}
	}
	adcHeaderChk();
}
#endif

void	DMA_INT3_IRQHandler(void)
{
#if 0
		P1->OUT |= BIT4;
		MAP_DMA_disableChannel(USBDMACH);
		MAP_DMA_disableInterrupt(INT_DMA_INT3);
		MAP_Interrupt_disableInterrupt(INT_DMA_INT3);
//		MAP_SPI_clearInterruptFlag(EUSCI_B0_BASE,EUSCI_B_IFG_RXIFG);
		MAP_DMA_clearInterruptFlag(USBDMACH);
#else
	if(writeSRAMDMAStatus == ON)
	{
		MAP_DMA_disableChannel(SRAMDMACH);
		MAP_DMA_disableInterrupt(INT_DMA_INT3);
		MAP_Interrupt_disableInterrupt(INT_DMA_INT3);
		MAP_SPI_clearInterruptFlag(EUSCI_A1_BASE,EUSCI_A_IFG_RXIFG);
		MAP_DMA_clearInterruptFlag(SRAMDMACH);

//		MAP_DMA_disableChannel(SRAMDMACH);
//		MAP_DMA_disableInterrupt(INT_DMA_INT3);
//		MAP_Interrupt_disableInterrupt(INT_DMA_INT3);
//		MAP_SPI_clearInterruptFlag(EUSCI_A1_BASE,EUSCI_A_IFG_RXIFG);
//		MAP_DMA_clearInterruptFlag(SRAMDMACH);
//		delay1us();
		writeSRAMDMAStatus = SRAMWRITEDMASTATUS00;
		sramCSHi();
	}
	if(readSRAMDMAStatus != 0)
	{
		MAP_DMA_disableChannel(SRAMDMACH);
		MAP_DMA_disableInterrupt(INT_DMA_INT3);
		MAP_Interrupt_disableInterrupt(INT_DMA_INT3);
		MAP_SPI_clearInterruptFlag(EUSCI_A1_BASE,EUSCI_A_IFG_RXIFG);
		MAP_DMA_clearInterruptFlag(SRAMDMACH);
		readSRAMDMAFlag = ON;
	}
#endif
}

/* Port4 ISR */
void	PORT4_IRQHandler(void)
{
	if(P4->IFG & BIT2)											// トリガ入力
	{
		P4->IFG &= ~BIT2;
	}
	if(P4->IFG & BIT3)											// 時刻同期入力
	{
		P4->IFG &= ~BIT3;
		if(syncRsvFlag == ON)
		{
			P7->OUT |= BIT6;
			testData = 0;
			syncWaitFlag = OFF;
			syncRsvFlag = OFF;
			adBufAdr = 0;
			usbStartCount = 0;
			usbStartAdr = 0;
			testUsbData = 0;
			sramSaveStartAdr = 0;
			sramLoadStartAdr = 0;
			sramSaveCount = 0;
			sramLoadCount = 0;
			sramLoadOK = OFF;
			sramSaveStep = STEP0;
			sramLoadStep = STEP0;
			sdBufAdr = 0;

			RTCCTL0_H = RTCKEY_H;

			RTCCTL1 |= RTCHOLD;									// RTC hold
			RTCPS = 0;
			RTCCTL1 &= ~(RTCHOLD);								// Start RTC

			RTCCTL0_H = 0;

			P7->OUT &= ~BIT6;
		}
	}
	if(P4->IFG & BIT4)											// 1PPS入力
	{
		if(syncStartCounter == 0)
		{
			sync1PPSFlag = ON;
		} else
		{
			syncStartCounter--;
			if(unitType == MASTER)
			{
				Timer2Set(TIME_4S);
			}
		}
		P4->IFG &= ~BIT4;
	}
	if(P4->IFG & BIT6)											// USB/SPI CMD REQ
	{
		dataStart = OFF;
		cmdStep = STEP1;
		dataSendStep = STEP0;
		P4->IFG &= ~BIT6;
	}
	if(P4->IFG & BIT7)											// USB/SPI CMD ACK
	{
		P4->IFG &= ~BIT7;
	}
}

/* Port6 ISR */
void	PORT6_IRQHandler(void)
{
	if(P6->IFG & BIT7)											// AD DRDY Interrupt
	{
		if(adStart == ON)
		{
//			adcDMARead();
//			adcRead();
//			resetDMA2();
//ledD4OnOff();
		}
		P6->IFG &= ~BIT7;
	}
}

void TA0_0_IRQHandler(void)
{
	TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}

void	TA1_0_IRQHandler(void)
{
	TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
	clockCount++;
}

void	TA1_N_IRQHandler(void)
{
	switch(TA1IV)
	{
		case 8:
			if(syncMeasureStart == ON)
			{
				clockCountOld = clockCount;
				clockCount = 0;
				capCountOld = capCount;
				capCount = TIMER_A1->CCR[4];
				TA1CTL |= TACLR;
//				syncMeasureStart = OFF;
			}
			__no_operation();
			TA0CCTL4 &= ~CCIFG;									//
		break;

		default:
			__no_operation();
		break;
	}
}

void	TA2_0_IRQHandler(void)
{
	TIMER_A2->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
	timeCount();
}

void	T32_INT1_IRQHandler(void)
{
	TIMER32_1->INTCLR |= BIT0;									// Clear Timer32 interrupt flag
}

void	WDT_A_IRQHandler(void)
{
	__no_operation();
	error();
}

/* RTC ISR */
void	RTC_C_IRQHandler(void)
{
#if 1
	uint32_t status;

	rtcGet();

//	if((dateTime.minutes == 0x59) && (dateTime.seconds == 0x59))
//	{
//		sync1PPSFlag = OFF;
//		syncStartFlag = ON;
//	}

	status = MAP_RTC_C_getEnabledInterruptStatus();
	MAP_RTC_C_clearInterruptFlag(status);

//	if (status & RTC_C_CLOCK_READ_READY_INTERRUPT)
//	{
//	}

//	if (status & RTC_C_TIME_EVENT_INTERRUPT)
//	{
		/* Interrupts every minute - Set breakpoint here */
//		__no_operation();
//		newTime = MAP_RTC_C_getCalendarTime();

//	}

//	if (status & RTC_C_CLOCK_ALARM_INTERRUPT)
//	{
		/* Interrupts at 10:04pm */
//		__no_operation();
//	}
#else
	if (RTCCTL0 & RTCTEVIFG)
	{
		RTCCTL0_H = RTCKEY_H ;
		RTCCTL0_L &= ~RTCTEVIFG;
		RTCCTL0_H = 0;
//		TA3CTL &= TACLR;
//		Event60sTimeSet();										// 60s Event Timer Set
//		AD.Start = ON;											// AD Start Flag
	}
#endif
}

void	EUSCIA3_IRQHandler(void)
{
	if (UCA3IFG & UCRXIFG)
	{
		if(gpsPoint > 0)
		{
			gpsTempBuffer[gpsPoint] = (uchar)UCA3RXBUF;
			if((gpsTempBuffer[gpsPoint] == LF) && (gpsTempBuffer[gpsPoint-1] == CR))
			{
				if((memcmp(&gpsTempBuffer[1], gprmcCMD, sizeof(gprmcCMD))) == 0)
				{
					if(gpsCtr == 0)
					{
						gpsLength = gpsPoint + 1;
						memcpy(gpsBuffer, gpsTempBuffer, gpsLength);
					}
				} else
				if((memcmp(&gpsTempBuffer[1], gpggaCMD, sizeof(gpggaCMD))) == 0)
				{
					if(gpsCtr == 1)
					{
						gpsLength = gpsPoint + 1;
						memcpy(gpsBuffer, gpsTempBuffer, gpsLength);
					}
				}
				gpsPoint = 0;
			} else
			{
				gpsPoint++;
			}
		} else
		{
			gpsTempBuffer[0] = (uchar)UCA3RXBUF;
			if(gpsTempBuffer[0] == '$')
			{
				gpsPoint++;
			}
		}
	}
}

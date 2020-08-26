/****************************************
 *	main.c								*
 ****************************************/

#include	<Lib/driverlib.h>
#include	<string.h>
#include	"msp.h"
#include	"define.h"
#include	"memory.h"
#include	"disp.h"

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif

static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[16];  // 8 primary and 8 alternate

extern void MAP_SPI_clearInterruptFlag(uint32_t moduleInstance, uint_fast8_t mask);
extern	DMA_ControlTable adRXDmaSeq[ADRX_TASKS];
extern	DMA_ControlTable adTXDmaSeq[ADTX_TASKS];
extern	DMA_ControlTable sdRXDma00Seq[RX1_TASKS];
extern	DMA_ControlTable sdRXDma01Seq[RX2_TASKS];
extern	DMA_ControlTable sdRXDma02Seq[RX3_TASKS];
extern	DMA_ControlTable sdRXDma03Seq[RX4_TASKS];
extern	DMA_ControlTable sdRXDma04Seq[RX5_TASKS];
extern	DMA_ControlTable sdRXDma05Seq[RX6_TASKS];
extern	DMA_ControlTable sdRXDma06Seq[RX7_TASKS];
extern	DMA_ControlTable sdRXDma07Seq[RX8_TASKS];
extern	DMA_ControlTable sdTXDma00Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma01Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma02Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma03Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma04Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma05Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma06Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma07Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma10Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma11Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma12Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma13Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma14Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma15Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma16Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma17Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma20Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma21Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma22Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma23Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma24Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma25Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma26Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma27Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma30Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma31Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma32Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma33Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma34Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma35Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma36Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma37Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma40Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma41Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma42Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma43Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma44Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma45Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma46Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma47Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma50Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma51Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma52Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma53Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma54Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma55Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma56Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma57Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma60Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma61Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma62Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma63Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma64Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma65Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma66Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma67Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma70Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma71Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma72Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma73Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma74Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma75Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma76Seq[SDTXTASKS];
extern	DMA_ControlTable sdTXDma77Seq[SDTXTASKS];
extern	DMA_ControlTable usbTXDmaData00Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma00Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma02Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma04Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma08Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma16Seq[USBTXTASKS];
extern	DMA_ControlTable sramRXDma00Seq[SRAMRXTASKS00];
extern	DMA_ControlTable sramRXDma01Seq[SRAMRXTASKS01];
extern	DMA_ControlTable sramTXDma00Seq[SRAMTXTASKS];
extern	DMA_ControlTable sramTXDma10Seq[SRAMTXTASKS];
extern	DMA_ControlTable sramTXDma20Seq[SRAMTXTASKS];
extern	DMA_ControlTable sramTXDma30Seq[SRAMTXTASKS];
extern	DMA_ControlTable sramTXDma40Seq[SRAMTXTASKS];
extern	DMA_ControlTable sramTXDma50Seq[SRAMTXTASKS];
extern	DMA_ControlTable sramTXDma60Seq[SRAMTXTASKS];
extern	DMA_ControlTable sramTXDma70Seq[SRAMTXTASKS];
extern	void	usbDMAHeaderInit(void);
extern	void	usbDMASend(void);
extern	void	usbSend(void);
extern	uint	adcSVRead(uchar data1, uchar data2);
extern	void	adcSVWrite(uchar data1, uchar data2);
extern	void	sramSaveCtr(void);
extern	void	sramLoadCtr(void);
extern	void	sdCheck(void);
extern	uchar	sdSendByte(uchar spiDataByte);
extern	uchar	sdGetResponse(void);
extern	uchar	sdCheckBusy(void);
extern	uchar	sdWRBusyChk(void);
extern	uchar	sdWRStsRead(void);
//extern	void	sdCompMain(void);
extern	uchar	sdBlockRead(long blocknum);
extern	uchar	sdBlockWrite(long blocknum, uchar *buf);
extern	void	initSd(void);
extern	void	delay1us(void);
extern	void	delay5us(void);
extern	void	delay10us(void);
extern	void	delay30us(void);
extern	void	delay100us(void);
extern	void	delay1ms(void);
extern	void	delay10ms(void);
extern	void	delay50ms(void);
extern	void	delay100ms(void);
extern	void	resetEventTime01MS(void);
extern	void	resetEventTime02MS(void);
extern	void	resetEventTime05MS(void);
extern	void	resetEventTime10MS(void);
extern	void	resetEventTime16MS(void);
extern	void	resetEventTime20MS(void);
extern	void	resetEventTime40MS(void);
extern	void	resetEventTime100MS(void);
extern	void	setEventTime200MS(void);
extern	void	resetEventTime1S(void);
extern	void	resetEventTime60S(void);
extern	void	setEventTime01MS(void);
extern	void	setEventTime02MS(void);
extern	void	setEventTime05MS(void);
extern	void	setEventTime10MS(void);
extern	void	setEventTime16MS(void);
extern	void	setEventTime20MS(void);
extern	void	setEventTime40MS(void);
extern	void	setEventTime100MS(void);
extern	void	setEventTime1S(void);
extern	void	setEventTime60S(void);
extern	void	timeCount(void);
extern	void	timeSyncOut(void);
extern	void	rtcGet(void);
extern	void	dayTimeCheck(void);
extern	void	errCheck(void);
extern	void	flashRead(void);
extern	void	flashWrite(void);
extern	void	flashCtr(void);
extern	void	sdErrDisp(void);
extern	void	sdBusyCheck(void);
//extern	void	testSdWrite(void);
extern	void	sdWriteCtr(void);
//extern	void	sdWriteTest(void);
extern	void	sdRWTestMain(void);
extern	void	commandDataTrans(void);
extern	void	commandSelect(void);

void	usbDMAWrite(void);
void	adcDMARead(void);
void	readSDBlock(void);
void	writeSDBlock(void);
void	keyCheck(void);
void	ledD1Off(void);
void	ledD1On(void);
void	ledD1OnOff(void);
void	ledD2Off(void);
void	ledD2On(void);
void	ledD2OnOff(void);
void	ledD3Off(void);
void	ledD3On(void);
void	ledD3OnOff(void);
void	ledD4Off(void);
void	ledD4On(void);
void	ledD4OnOff(void);
void	led2Off(void);
void	led2On(void);
void	led2OnOff(void);
void	testPort1Off(void);
void	testPort1On(void);
void	testPort1OnOff(void);
void	error(void);
void	SDSPISel(void);
void	initBuf(void);
void	initADCSet(void);
void	initTimer(void);
void	initPort(void);
void	initDMA(void);
void	initSpi(void);
void	initRTC(void);
void	initFlash(void);
void	initClk(void);

void	usbDMAWrite(void)
{
	/*
	* Start SPI read
	*/
	MAP_DMA_setChannelScatterGather(USBDMACHSPI,USBTXTASKS,(void*)&usbTXDma00Seq[0],0);

	MAP_DMA_clearInterruptFlag(USBDMACHSPI & 0x0F);

	/* Assigning/Enabling Interrupts */
	MAP_DMA_enableInterrupt(INT_DMA_INT3);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
	MAP_DMA_enableChannel(USBDMACH);

	/*
	* The DMA channel3 is set to trigger off of theEUSCIB0RX IFG.  This
	* did not really happen but is forced manually to start the
	* scatter-gather sequence.  Because the setting is 'memory'
	* scatter-gather all triggers (transitions) from primary to
	* auxiliary actions will be automatic.  The auxiliary action triggers
	* are defined in the task list.
	*/
//	writeUSBExe = ON;
	EUSCI_A1_SPI->IFG |= EUSCI_A_IFG_RXIFG;
}

void	adcDMARead(void)
{
	/*
	* Start SPI read
	*/

	MAP_DMA_setChannelScatterGather(ADDMACHSPI,ADRX_TASKS,(void*)&adRXDmaSeq[0],0);
	MAP_DMA_clearInterruptFlag(ADDMACHSPI & 0x0F);

	/* Assigning/Enabling Interrupts */
	MAP_DMA_enableInterrupt(INT_DMA_INT2);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT2);
	MAP_DMA_enableChannel(ADDMACH);

	P6OUT &= ~BIT2;

	/*
	* The DMA channel3 is set to trigger off of theEUSCIB0RX IFG.  This
	* did not really happen but is forced manually to start the
	* scatter-gather sequence.  Because the setting is 'memory'
	* scatter-gather all triggers (transitions) from primary to
	* auxiliary actions will be automatic.  The auxiliary action triggers
	* are defined in the task list.
	*/
	EUSCI_B2_SPI->IFG |= EUSCI_B_IFG_RXIFG;
}

void	readSDBlock(void)
{
	/*
	* Start SPI read
	*/
	MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&sdRXDma00Seq[0],0);
//	MAP_DMA_setChannelScatterGather(SDDMACHSPI,RX1_TASKS,(void*)&rxDma1Seq[0],0);
	MAP_DMA_clearInterruptFlag(SDDMACHSPI & 0x0F);

	/* Assigning/Enabling Interrupts */
	MAP_DMA_enableInterrupt(INT_DMA_INT1);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
	MAP_DMA_enableChannel(SDDMACH);

	/*
	* The DMA channel3 is set to trigger off of theEUSCIB0RX IFG.  This
	* did not really happen but is forced manually to start the
	* scatter-gather sequence.  Because the setting is 'memory'
	* scatter-gather all triggers (transitions) from primary to
	* auxiliary actions will be automatic.  The auxiliary action triggers
	* are defined in the task list.
	*/
	EUSCI_B3_SPI->IFG |= EUSCI_B_IFG_RXIFG;
	readSDDMAStatus = SDREADDMASTATUS01;
}

void	writeSDBlock(void)
{
	/*
	* Start SPI read
	*/
	MAP_DMA_setChannelScatterGather(SDDMACHSPI,SDTXTASKS,(void*)&sdTXDma00Seq[0],0);
//	MAP_DMA_setChannelScatterGather(SDDMACHSPI,TX1_TASKS,(void*)&txDma1Seq[0],0);
	MAP_DMA_clearInterruptFlag(SDDMACHSPI & 0x0F);

	/* Assigning/Enabling Interrupts */
	MAP_DMA_enableInterrupt(INT_DMA_INT1);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
	MAP_DMA_enableChannel(SDDMACH);

	/*
	* The DMA channel3 is set to trigger off of theEUSCIB0RX IFG.  This
	* did not really happen but is forced manually to start the
	* scatter-gather sequence.  Because the setting is 'memory'
	* scatter-gather all triggers (transitions) from primary to
	* auxiliary actions will be automatic.  The auxiliary action triggers
	* are defined in the task list.
	*/
	EUSCI_B3_SPI->IFG |= EUSCI_B_IFG_RXIFG;
	writeSDDMAStatus = SDWRITEDMASTATUS01;
	writeSDExe = ON;
}

void	keyCheck(void)
{
	if(!(P1->IN & BIT1))
	{
		if(keyCount == 3)
		{
			if(dataEnable == ON)
			{
				dataEnable = OFF;
			} else
			{
				dataEnable = ON;
			}
		}
		keyCount++;
	} else
	{
		keyCount = 0;
	}
}

void	ledD1Off(void)
{
	P10->OUT &= ~BIT5;											// LED D1(yellow)
}

void	ledD1On(void)
{
	P10->OUT |= BIT5;											// LED D1(yellow)
}

void	ledD1OnOff(void)
{
	P10->OUT ^= BIT5;											// LED D1(yellow)
}

void	ledD2Off(void)
{
	P2->OUT &= ~BIT6;											// LED D2(orange)
}

void	ledD2On(void)
{
	P2->OUT |= BIT6;											// LED D2(orange)
}

void	ledD2OnOff(void)
{
	P2->OUT ^= BIT6;											// LED D2(orange)
}

void	ledD3Off(void)
{
	P8->OUT &= ~BIT7;											// LED D3(red)
}

void	ledD3On(void)
{
	P8->OUT |= BIT7;											// LED D3(red)
}

void	ledD3OnOff(void)
{
	P8->OUT ^= BIT7;											// LED D3(red)
}

void	ledD4Off(void)
{
	P9->OUT &= ~BIT1;											// LED D3(green)
}

void	ledD4On(void)
{
	P9->OUT |= BIT1;											// LED D3(green)
}

void	ledD4OnOff(void)
{
	P9->OUT ^= BIT1;											// LED D3(green)
}

void	led2Off(void)
{
	P2->OUT &= ~BIT0;
}

void	led2On(void)
{
	P2->OUT |= BIT0;
}

void	led2OnOff(void)
{
	P2->OUT ^= BIT0;
}

void	testPort1Off(void)
{
	P1->OUT &= ~BIT0;
}

void	testPort1On(void)
{
	P1->OUT |= BIT0;
}

void	testPort1OnOff(void)
{
	P1->OUT ^= BIT0;
}

void	error(void)
{
	RSTCTL->RESET_REQ |= (0x6900 | RSTCTL_RESET_REQ_HARD_REQ);

	while (1)
	{
		__no_operation();										// For debugger
	}
}

void	SDSPISel(void)											// SPISelect
{
	//SPIB3(SD i/f)

	if(SDCard.Speed == SD_LOW_SPEED)							// LowSpeed:0/HighSpeed:1
	{
		//48MHz=1/48000000=20.8ns*0xf0(240)=4.992us=sclk(200kHz)
		EUSCI_B3->CTLW0 |= EUSCI_B_CTLW0_SWRST;					// Put eUSCI state machine in reset
		EUSCI_B3->BRW = SD_CLK_PULS_INIT;						// /2,fBitClock = fBRCLK/(UCBRx+1).
		EUSCI_B3->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;				// Initialize USCI state machine
	} else
	{
		//48MHz=1/48000000=20.8ns*0x10(16)=333ns=sclk(3MHz)
		EUSCI_B3->CTLW0 |= EUSCI_B_CTLW0_SWRST;					// Put eUSCI state machine in reset
		EUSCI_B3->BRW = SD_CLK_PULS;							// /2,fBitClock = fBRCLK/(UCBRx+1).
		EUSCI_B3->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;				// Initialize USCI state machine
	}
}

void	initBuf(void)
{
//	memset(adWorkBuf, 0x00, AD_DATA_COUNT);
//	memset(adBuf, 0x00, (SD_BLOCK_SIZE*AD_DATA_COUNT*SURFACE));
	memset(sdReadBuf, 0x00, SD_READ_SIZE);
	memset(sdWorkReadBuf, 0x00, SD_BLOCK_SIZE);
//	memset(sdWriteBuf, 0x00, (SD_BLOCK_SIZE * SD_WRITE_SECTOR*2));
//	memset(usbWriteBuf, 0x00, (usbWriteCountSV));
//	memset(usbHeaderBuf, 0x00, (USB_HEADER_LENGTH));
//	memset(recordIndexBuf, 0x00, INDEX_POINTER);
//	usbDMAHeaderInit();
}

void	initADCSet(void)
{

//	delay100ms();
//	P9OUT &= ~BIT3;												// AD Reset
	P5OUT &= ~BIT5;												// AD Reset
	P9OUT &= ~BIT2;												// AD Reset
	delay100ms();
	delay100ms();
	delay100ms();
	P5OUT |= BIT5;												//
	P9OUT |= BIT2;												//

	delay50ms();
//	readAdRegStatus.r14 = adcSVRead(0x94, 0x00);				// Read 0x94
//	delay1ms();
//	adcSVWrite(0x00, 0x00);										// Write 0x00 B7-6 PGA 00=x1 01=x2 10=x4 11=x8
//	delay50ms();
//	adcSVWrite(0x01, 0x00);										// Write 0x01 B7-6 PGA 00=x1 01=x2 10=x4 11=x8
//	delay50ms();
//	adcSVWrite(0x02, 0x00);										// Write 0x02 B7-6 PGA 00=x1 01=x2 10=x4 11=x8
//	delay50ms();
//	adcSVWrite(0x03, 0x00);										// Write 0x03 B7-6 PGA 00=x1 01=x2 10=x4 11=x8
//	delay50ms();
//	adcSVWrite(0x04, 0x00);										// Write 0x04 B7-6 PGA 00=x1 01=x2 10=x4 11=x8
//	delay50ms();
//	adcSVWrite(0x05, 0x00);										// Write 0x05 B7-6 PGA 00=x1 01=x2 10=x4 11=x8
//	delay50ms();
//	adcSVWrite(0x06, 0x00);										// Write 0x06 B7-6 PGA 00=x1 01=x2 10=x4 11=x8
//	delay50ms();
//	adcSVWrite(0x07, 0x00);										// Write 0x07 B7-6 PGA 00=x1 01=x2 10=x4 11=x8
//	delay50ms();
	adcSVWrite(0x14, 0xc0);										// Write 0x14 B6 0=LowPow 1=hiReso
	delay50ms();
	readAdRegStatus.r00 = adcSVRead(0x80, 0x00);				// Read 0x80
	delay50ms();
	readAdRegStatus.r01 = adcSVRead(0x81, 0x00);				// Read 0x81
	delay50ms();
	readAdRegStatus.r02 = adcSVRead(0x82, 0x00);				// Read 0x82
	delay50ms();
	readAdRegStatus.r03 = adcSVRead(0x83, 0x00);				// Read 0x83
	delay50ms();
	readAdRegStatus.r04 = adcSVRead(0x84, 0x00);				// Read 0x84
	delay50ms();
	readAdRegStatus.r05 = adcSVRead(0x85, 0x00);				// Read 0x85
	delay50ms();
	readAdRegStatus.r06 = adcSVRead(0x86, 0x00);				// Read 0x86
	delay50ms();
	readAdRegStatus.r07 = adcSVRead(0x87, 0x00);				// Read 0x87
	delay50ms();
	readAdRegStatus.r09 = adcSVRead(0x89, 0x00);				// Read 0x89
	delay50ms();
	readAdRegStatus.r0a = adcSVRead(0x8a, 0x00);				// Read 0x8a
	delay50ms();
	readAdRegStatus.r0b = adcSVRead(0x8b, 0x00);				// Read 0x8b
	delay50ms();
	readAdRegStatus.r0c = adcSVRead(0x8c, 0x00);				// Read 0x8c
	delay50ms();
	readAdRegStatus.r0d = adcSVRead(0x8d, 0x00);				// Read 0x8d
	delay50ms();
	readAdRegStatus.r0e = adcSVRead(0x8e, 0x00);				// Read 0x8e
	delay50ms();
	readAdRegStatus.r0f = adcSVRead(0x8f, 0x00);				// Read 0x8f
	delay50ms();
	readAdRegStatus.r10 = adcSVRead(0x90, 0x00);				// Read 0x90
	delay50ms();
	readAdRegStatus.r14 = adcSVRead(0x94, 0x00);				// Read 0x94
	delay50ms();
	readAdRegStatus.r11 = adcSVRead(0x91, 0x00);				// Read 0x91
	delay50ms();
	readAdRegStatus.r12 = adcSVRead(0x92, 0x00);				// Read 0x92
	delay50ms();
	readAdRegStatus.r13 = adcSVRead(0x93, 0x00);				// Read 0x93
	delay50ms();
	readAdRegStatus.r15 = adcSVRead(0x95, 0x00);				// Read 0x95
	delay50ms();
	readAdRegStatus.r16 = adcSVRead(0x96, 0x00);				// Read 0x96
	delay50ms();
	readAdRegStatus.r17 = adcSVRead(0x97, 0x00);				// Read 0x97
	delay50ms();
	readAdRegStatus.r18 = adcSVRead(0x98, 0x00);				// Read 0x98
	delay50ms();
	readAdRegStatus.r19 = adcSVRead(0x99, 0x00);				// Read 0x99
	delay50ms();
	readAdRegStatus.r1a = adcSVRead(0x9a, 0x00);				// Read 0x9a
	delay50ms();
	readAdRegStatus.r08 = adcSVRead(0x88, 0x00);				// Read 0x08
	delay50ms();
	readAdRegStatus.r13 = adcSVRead(0x93, 0x00);				// Read 0x93
	delay50ms();
	adcSVWrite(0x13, 0x90);										// Write 0x11 B6 0=LowPow 1=hiReso
	delay50ms();
//	adcSVWrite(0x11, 0x24);										// Write 0x11 B6=0:LowPower 1:High reso
//	delay1ms();
//	adcSVWrite(0x14, 0xc2);										// Write 0x14 DataOutPut DOUT0-4=>DOUT0
//	delay1ms();
//	rxTestBufferCh1 = adcSVRead(0x94, 0x00);					// Read 0x14
//	adcSVWrite(0x08, 0x40);										// Write 0x08 Disable Ch4-7
//	adcSVWrite(0x13, 0x90);										// Write 0x11 B6 0=LowPow 1=hiReso
//	P9OUT |= BIT2;												//
//	P5OUT |= BIT5;												//
//	P6OUT |= BIT6;												// P6.0 SRAM3 CS
}

void	initTimer(void)
{
	/* Timer32 set up in periodic mode, 32-bit, no pre-scale */
	TIMER32_1->CONTROL =   TIMER32_CONTROL_SIZE | TIMER32_CONTROL_MODE;
	/* Load Timer32 counter with period = 0x20000 */
//	TIMER32_1->LOAD= TIMER1SCNT;
//	TIMER32_1->LOAD= 12000000;
	TIMER32_1->LOAD= 48000000;
	NVIC->ISER[0] = 1 << ((T32_INT1_IRQn) & 31);
//	SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;						// Enable sleep on exit from ISR
	/* Start Timer32 with interrupt enabled*/
	TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE | TIMER32_CONTROL_IE;

	TA0CCTL0 = CCIE;											// TA3CCR0 interrupt enabled
	TA0CCR0 = 48000;
	TA0CTL = TASSEL__SMCLK | MC__UP;							// SMCLK, UP mode
	NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);

	TA1CCTL4 |= CM_1 | CCIS_0 | CCIE | CAP| SCS;
																// Capture rising edge,
																// Use CCI4A=ACLK,
																// Synchronous capture,
																// Enable capture mode,
																// Enable capture interrupt

	TA1CCTL0 = CCIE;
//	TA0CCR0 = TIMER1MSCNT;										// 16ビット以内にするため
	TA1CCR0 = 47997;											// 16ビット以内にするため

	TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;					// SMCLK, UP mode
																// Start timer in continuous mode
//	SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;						// Enable sleep on exit from ISR
	NVIC->ISER[0] = 1 << ((TA1_0_IRQn) & 31);
	NVIC->ISER[0] = 1 << ((TA1_N_IRQn) & 31);

//	TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;						// TACCR0 interrupt enabled
//	TIMER_A0->CCR[0] = 12000;
//	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |					// SMCLK, continuous mode
//	TIMER_A_CTL_ID_0 | TIMER_A_CTL_MC_1;

	TIMER_A2->CCTL[0] = TIMER_A_CCTLN_CCIE;						// TACCR0 interrupt enabled
	TIMER_A2->CCR[0] = 12000;
	TIMER_A2->CTL = TIMER_A_CTL_SSEL__SMCLK |					// SMCLK, continuous mode
	TIMER_A_CTL_ID_0 | TIMER_A_CTL_MC_1;
}

void	initPort(void)
{
	P1->DIR = 0;												// P1
	P1->OUT = 0;
	P2->DIR = 0;												// P2
	P2->OUT = 0;
	P3->DIR = 0;												// P3
	P3->OUT = 0;
	P4->DIR = 0;												// P4
	P4->OUT = 0;
	P5->DIR = 0;												// P5
	P5->OUT = 0;
	P6->DIR = 0;												// P6
	P6->OUT = 0;
	P7->DIR = 0;												// P7
	P7->OUT = 0;
	P8->DIR = 0;												// P8
	P8->OUT = 0;
	P9->DIR = 0;												// P9
	P9->OUT = 0;
	P10->DIR = 0;												// P10
	P10->OUT = 0;

	P1->OUT |= BIT4 + BIT1;
	P1->REN |= BIT1;
	P1->DIR |= BIT4 + BIT0;										// P1 Test Port
	P1->SEL0 = 0;
	P1->SEL1 = 0;

	P2->DIR |= BIT4;											// P2
	P2->OUT |= BIT4;
	P2->SEL0 = 0;
	P2->SEL1 = 0;

	P3->SEL0 = 0;
	P3->SEL1 = 0;

	P4->DIR = BIT0;												// P4.0 SRAM4 CS
	P4->OUT = BIT0;

	P4->DIR |= BIT2;								// P4.2 ACLK/P4.3 MCLK/P4.4 HSMCLK output
	P4->SEL0 = BIT2;
//	P4->DIR |= BIT2 | BIT3 | BIT4;								// P4.2 ACLK/P4.3 MCLK/P4.4 HSMCLK output
//	P4->SEL0 = BIT2 | BIT3 | BIT4;
	P4->SEL1 = 0;

	P4->REN |= BIT3;											// P4.3 TimeSyncIn PullDown
	P4->REN |= BIT6;											// P4.6 USB/SPI GPIO2 PullDown
//	P4->REN |= BIT5 + BIT6;
//	P4->IES = BIT5;												// Interrupt on high-to-low transition
	P4->IES &= ~(BIT7 + BIT6 + BIT4 + BIT3 + BIT2);				// Interrupt on low-to-high transition
	P4->IFG = 0;												// Clear all P4 interrupt flags
	P4->IE = BIT2 + BIT3 + BIT4 + BIT6;										// Enable interrupt for P4
//	P4->IE = BIT5 + BIT6;										// Enable interrupt for P4

	P5->DIR |= BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1+BIT0;			// PowerSW On P5.6 GPS P5.5 AD7779 P5.4 USB HUB
	P5->OUT |= BIT6+BIT4+BIT3+BIT2+BIT1+BIT0;					// PowerSW On P5.3 SD Card P5.2 SRAM2 P5.1 SRAM1 P5.0 SPI/USB
	P5->SEL0 = 0;
	P5->SEL1 = 0;

	P6->SEL0 |= BIT3+BIT4+BIT5;									// P6.3,4,5 option select
	P6->SEL1 = 0;												// P6.3,4,5 option select

    /* Configure CS pin P6.1 */
	P6->OUT |= BIT6+BIT2+BIT0;									// P6.0 SRAM3 CS
	P6->DIR |= BIT6+BIT2+BIT0;
	P6->IES = BIT7;												// Interrupt on high-to-low transition
	P6->IFG = 0;												// Clear all P6 interrupt flags
	P6->IE = BIT7;												// Enable interrupt for P6.1
	P6->SEL0 = 0;
	P6->SEL1 = 0;

	P7->DIR |= BIT1+BIT0;										// PowerSW On P7.1 SRAM3 P7.0 SRAM4 P7.4 TP13
	P7->OUT |= BIT1+BIT0;

	if(unitType == MASTER)
	{
		P7->DIR |= BIT5+BIT6;									// P7.5 時刻同期出力 P7.6 トリガ出力
	} else
	{
		P7->DIR &= ~BIT5+BIT6;									// P7.5 時刻同期出力 P7.6 トリガ出力
	}

//	P7->OUT &= ~BIT5;
	P7->SEL0 = 0;
	P7->SEL1 = 0;
	P7SEL0 |= BIT4;												// TA1.4 input capture pin, second function
	P7DIR &= ~BIT4;
	P7REN |= BIT4;												//

	P8->OUT |= BIT2 + BIT5;										// 
	P8->DIR |= BIT2 + BIT5;										// 
	P8->SEL0 = 0;
	P8->SEL1 = 0;

	P9->DIR |= BIT3+BIT2;										// P9 AD Reset
	P9->SEL0 = 0;
	P9->SEL1 = 0;

	P10->DIR |= BIT0;											// P10
	P10->OUT |= BIT0;
	P10->SEL0 = 0;
	P10->SEL1 = 0;

//LED Set
	P10->DIR |= BIT5;											// LED D1(yellow)
	P2->DIR |= BIT6;											// LED D2(orange)
	P8->DIR |= BIT7;											// LED D3(red)
	P9->DIR |= BIT1;											// LED D3(green)
}

void	initDMA(void)
{
	/* Configuring DMA module */
	MAP_DMA_enableModule();
	MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);
	MAP_DMA_assignChannel(ADDMACHSPI);
	MAP_DMA_assignChannel(SDDMACHSPI);
//	MAP_DMA_assignChannel(USBDMACHSPI);
	MAP_DMA_assignChannel(SRAMDMACHSPI);

}

void	initSpi(void)
{
//	UCA1SPI(SRAM)
	P2->SEL0 |= BIT1+BIT2+BIT3;									// P2.1,2,3 option select
	P2->SEL1 = 0;												// P2.1,2,3 option select

	EUSCI_A1->CTLW0 |= EUSCI_A_CTLW0_SWRST;						// Put eUSCI state machine in reset
	EUSCI_A1->CTLW0 = EUSCI_A_CTLW0_SWRST |						// Remain eUSCI state machine in reset
		EUSCI_A_CTLW0_MST |										// Set as SPI master
		EUSCI_A_CTLW0_SYNC |									// Set as synchronous mode
		EUSCI_A_CTLW0_CKPH |									// Set clock polarity high
		EUSCI_A_CTLW0_MSB;										// MSB first

	EUSCI_A1->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;				// SMCLK
	EUSCI_A1->BRW = SRAM_CLK_PULS;								// /2,fBitClock = fBRCLK/(UCBRx+1).
	EUSCI_A1->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;					// Initialize USCI state machine
//	UCA1IFG |= EUSCI_A_IFG_TXIFG;								// Clear TXIFG flag

//	UCA3SPI(GPS)
	P9->SEL0 |= BIT6+BIT7;										// P9.6,7 option select
	P9->SEL1 = 0;												// P9.6,7 option select

	EUSCI_A3->CTLW0 |= EUSCI_A_CTLW0_SWRST;						// Put eUSCI state machine in reset
	EUSCI_A3->CTLW0 = EUSCI_A_CTLW0_SWRST |						// Remain eUSCI state machine in reset
		EUSCI_A_CTLW0_SSEL__SMCLK;								// Configure eUSCI clock source for SMCLK
	// Baud Rate calculation									// 48000000/16/9600=312.5
	EUSCI_A3->BRW = 312;										// Using baud rate calculator UCBRx=312
	EUSCI_A3->MCTLW = (8 << EUSCI_A_MCTLW_BRF_OFS) |			// UCBRF=8=ROUNDDOWN((48000000/9600/16-int(48000000/9600/16))*16)
		0x0000 |												// UCBRSx=0x00=(48000000/9600-INT(48000000/9600))=>TecRef Table24-4
		EUSCI_A_MCTLW_OS16;
	EUSCI_A3->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;					// Initialize eUSCI
	EUSCI_A3->IFG &= ~EUSCI_A_IFG_RXIFG;						// Clear eUSCI RX interrupt flag
	EUSCI_A3->IE |= EUSCI_A_IE_RXIE;							// Enable USCI_A3 RX interrupt

//	UCB0SPI(SPI USB)
	P1->SEL0 |= BIT5+BIT6+BIT7;									// P1.5,6,7 option select
	P1->SEL1 = 0;												// P1.5,6,7 option select

	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;						// Put eUSCI state machine in reset
	EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST |						// Remain eUSCI state machine in reset
		EUSCI_B_CTLW0_MST |										// Set as SPI master
		EUSCI_B_CTLW0_SYNC |									// Set as synchronous mode
		EUSCI_B_CTLW0_CKPH |									// Set clock phase high
//		EUSCI_B_CTLW0_CKPL |									// Set clock polarity high
		EUSCI_B_CTLW0_MSB;										// MSB first

	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;				// SMCLK
	EUSCI_B0->BRW = USB_CLK_PULS;								// /2,fBitClock = fBRCLK/(UCBRx+1).
	EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;					// Initialize USCI state machine

//	UCB1SPI(AD)
	P6->SEL0 |= BIT3+BIT4+BIT5;									// P6.3,4,5 option select
	P6->SEL1 = 0;												// P6.3,4,5 option select

	EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST;						// Put eUSCI state machine in reset
	EUSCI_B1->CTLW0 = EUSCI_B_CTLW0_SWRST |						// Remain eUSCI state machine in reset
		EUSCI_B_CTLW0_MST |										// Set as SPI master
		EUSCI_B_CTLW0_SYNC |									// Set as synchronous mode
		EUSCI_B_CTLW0_CKPL |									// Set clock polarity high
		EUSCI_B_CTLW0_MSB;										// MSB first

	EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;				// SMCLK
	EUSCI_B1->BRW = AD_CLK_PULS;								// /2,fBitClock = fBRCLK/(UCBRx+1).
	EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;					// Initialize USCI state machine
//	EUSCI_B1->IFG &= ~EUSCI_B_IFG_RXIFG;						// Clear eUSCI RX interrupt flag
//	EUSCI_B1->IE |= EUSCI_B_IE_RXIE;							// Enable USCI_B1 RX interrupt

//	UCB2SPI(AD)
	P3->SEL0 |= BIT5+BIT6+BIT7;									// P3.5,6,7 option select
	P3->SEL1 = 0;												// P3.5,6,7 option select

	EUSCI_B2->CTLW0 |= EUSCI_B_CTLW0_SWRST;						// Put eUSCI state machine in reset
	EUSCI_B2->CTLW0 = EUSCI_B_CTLW0_SWRST |						// Remain eUSCI state machine in reset
		EUSCI_B_CTLW0_MST |										// Set as SPI master
		EUSCI_B_CTLW0_SYNC |									// Set as synchronous mode
		EUSCI_B_CTLW0_CKPL |									// Set clock polarity high
		EUSCI_B_CTLW0_MSB;										// MSB first

	EUSCI_B2->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;				// SMCLK
	EUSCI_B2->BRW = AD_CLK_PULS;								// /2,fBitClock = fBRCLK/(UCBRx+1).
	EUSCI_B2->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;					// Initialize USCI state machine

//	UCB3SPI(SD Card)
	P10->SEL0 |= BIT1+BIT2+BIT3;								// P10.1,2,3 option select
	P10->SEL1 = 0;												// P10.1,2,3 option select

	EUSCI_B3->CTLW0 |= EUSCI_B_CTLW0_SWRST;						// Put eUSCI state machine in reset
	EUSCI_B3->CTLW0 = EUSCI_B_CTLW0_SWRST |						// Remain eUSCI state machine in reset
		EUSCI_B_CTLW0_MST |										// Set as SPI master
		EUSCI_B_CTLW0_SYNC |									// Set as synchronous mode
		EUSCI_B_CTLW0_CKPL |									// Set clock polarity high
		EUSCI_B_CTLW0_MSB;										// MSB first

	EUSCI_B3->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;				// SMCLK
	EUSCI_B3->BRW = SD_CLK_PULS_INIT;							// /2,fBitClock = fBRCLK/(UCBRx+1).
	EUSCI_B3->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;					// Initialize USCI state machine
}

void	initRTC(void)
{
	/* Initializing RTC with current time as described in time in
	 * definitions section */
	if(rtcDefault)
	{
		MAP_RTC_C_initCalendar(&currentTime, RTC_C_FORMAT_BCD);
	} else
	{
		RTC_C->CTL0 = (RTC_C->CTL0 & ~RTC_C_CTL0_KEY_MASK) | RTC_C_KEY;

		BITBAND_PERI(RTC_C->CTL13, RTC_C_CTL13_HOLD_OFS) = 1;

		if (RTC_C_FORMAT_BCD)
			BITBAND_PERI(RTC_C->CTL13, RTC_C_CTL13_BCD_OFS) = 1;
		else
		BITBAND_PERI(RTC_C->CTL13, RTC_C_CTL13_BCD_OFS) = 0;

		RTC_C->TIM0 = (dateTime.minutes << RTC_C_TIM0_MIN_OFS) | dateTime.seconds;
		RTC_C->TIM1 = (dateTime.dayOfWeek << RTC_C_TIM1_DOW_OFS) | dateTime.hours;
		RTC_C->DATE = (dateTime.month << RTC_C_DATE_MON_OFS) | dateTime.day;
		RTC_C->YEAR = dateTime.year;

		BITBAND_PERI(RTC_C->CTL0, RTC_C_CTL0_KEY_OFS) = 0;
	}

	/* Setup Calendar Alarm for 10:04pm (for the flux capacitor) */
//	MAP_RTC_C_setCalendarAlarm(0x04, 0x22, RTC_C_ALARMCONDITION_OFF,
//			RTC_C_ALARMCONDITION_OFF);

	/* Specify an interrupt to assert every minute */
//	MAP_RTC_C_setCalendarEvent(RTC_C_CALENDAREVENT_MINUTECHANGE);
	MAP_RTC_C_setCalendarEvent(RTC_C_CALENDAREVENT_HOURCHANGE);

	/* Enable interrupt for RTC Ready Status, which asserts when the RTC
	 * Calendar registers are ready to read.
	 * Also, enable interrupts for the Calendar alarm and Calendar event. */
//	MAP_RTC_C_clearInterruptFlag(
//			RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT
//					| RTC_C_CLOCK_ALARM_INTERRUPT);
//	MAP_RTC_C_enableInterrupt(
//			RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT
//					| RTC_C_CLOCK_ALARM_INTERRUPT);

	MAP_RTC_C_enableInterrupt(
			RTC_C_TIME_EVENT_INTERRUPT);

	/* Start RTC Clock */
	MAP_RTC_C_startClock();
	//![Simple RTC Example]
}

void	initFlash(void)
{
	FlashCtl_setWaitState(FLASH_BANK0, 1);
	FlashCtl_setWaitState(FLASH_BANK1, 1);
	flashCtr();
}

void	initClk(void)
{
	// Get current power state, if it's not AM_LDO_VCORE0, error out
	if (PCM->CTL0 & PCM_CTL0_CPM_MASK != PCM_CTL0_CPM_0)
		error();

	while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
	PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
	while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
	if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
		error();												// Error if transition was not successful
	if ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_1)
		error();												// Error if device is not in AM1_LDO mode

	// Step 3: Configure Flash wait-state to 1 for both banks 0 & 1
	FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
		FLCTL_BANK0_RDCTL_WAIT_1;
	FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) |
		FLCTL_BANK1_RDCTL_WAIT_1;

	PJ->SEL0 |= BIT2 | BIT3;									// Configure PJ.2/3 for HFXT function
	PJ->SEL1 &= ~(BIT2 | BIT3);

	CS->KEY = CS_KEY_VAL ;										// Unlock CS module for register access
	CS->CTL2 |= CS_CTL2_HFXT_EN | CS_CTL2_HFXTFREQ_6 | CS_CTL2_HFXTDRIVE;
	while(CS->IFG & CS_IFG_HFXTIFG)
		CS->CLRIFG |= CS_CLRIFG_CLR_HFXTIFG;

	/* Select MCLK & HSMCLK = HFXT, no divider */
	CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK | CS_CTL1_SELS_MASK | CS_CTL1_DIVHS_MASK) |
		 CS_CTL1_SELM__HFXTCLK | CS_CTL1_SELS__HFXTCLK;

	CS->KEY = 0;												// Lock CS module from unintended accesses
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;									// Stop watchdog timer

	__disable_interrupt();
	initClk();

	initRTC();													// RTC起動
	initFlash();
	initPort();
	initSpi();
	initADCSet();
	initTimer();
	initSd();
	if(SDCard.Err_Flag == NG)
	{
		sdErrStatus = SD_INIT_ERR;
	}
	initDMA();
	initBuf();

	/* Configuring DMA module */
//	MAP_DMA_enableModule();
//	MAP_DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);
//	MAP_DMA_assignChannel(SDDMACHSPI);

	MAP_DMA_assignInterrupt(INT_DMA_INT1, SDDMACH);				/* Channel 3 is the ADC (RX) */
	MAP_DMA_assignInterrupt(INT_DMA_INT2, ADDMACH);				/* Channel 3 is the ADC (RX) */
//	MAP_DMA_assignInterrupt(INT_DMA_INT3, USBDMACH);			/* Channel 3 is the ADC (RX) */
	MAP_DMA_assignInterrupt(INT_DMA_INT3, SRAMDMACH);				/* Channel 3 is the ADC (RX) */

	// Enable Port 4 interrupt on the NVIC
	NVIC->ISER[1] = 1 << ((PORT4_IRQn) & 31);
	// Enable Port 6 interrupt on the NVIC
	NVIC->ISER[1] = 1 << ((PORT6_IRQn) & 31);

	NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
	//TA2
	NVIC->ISER[0] = 1 << ((TA2_0_IRQn) & 31);

	NVIC->ISER[0] = 1 << ((EUSCIA3_IRQn) & 31);					// Enable eUSCIA3 interrupt in NVIC module

	MAP_Interrupt_enableInterrupt(INT_RTC_C);
//	adStart = ON;
	sdStart = ON;
	sramStart = ON;
//	usbStart = ON;
	sync1PPSFlag = OFF;
	syncStartFlag = ON;
	syncMeasureStart = OFF;

	__enable_interrupt();

	while (1)
	{
//		timeSyncOut();
//		sramSaveCtr();
//		sramLoadCtr();
//		sdWriteCtr();
//		usbDMASend();
//		usbSend();
		if(eventTime01MS) {
			setEventTime01MS();
		}
		else if(eventTime02MS) {
			commandSelect();
			setEventTime02MS();
		}
		else if(eventTime05MS) {
			setEventTime05MS();
		}
		else if(eventTime10MS)
		{
			setEventTime10MS();
		}
		else if(eventTime16MS)
		{
			setEventTime16MS();
		}
		else if(eventTime20MS)
		{
			setEventTime20MS();
		}
		else if(eventTime40MS)
		{
		sdWriteCtr();
			setEventTime40MS();
		}
		else if(eventTime100MS)
		{
			errCheck();
			sdErrDisp();
			sdCheck();
			setEventTime100MS();
		}
		else if(eventTime200MS)
		{
			setEventTime200MS();
		}
		else if(eventTime1S)
		{
			dayTimeCheck();
			if((timeGet == ON) && (altGet == ON))
			{
//				ledD2OnOff();
			} else
			{
				rtcGet();
			}
//			ledD1OnOff();
//			if(testFlag == ON)
//			{
//			}
			setEventTime1S();
		}
		__sleep();
		__no_operation();
	}
}


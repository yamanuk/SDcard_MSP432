/****************************************
 *	sram.c								*
 ****************************************/

#include	<Lib/driverlib.h>
#include	"msp.h"
#include	<string.h>
#include	"define.h"
#include	"extern.h"

extern void MAP_SPI_clearInterruptFlag(uint32_t moduleInstance, uint_fast8_t mask);
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
extern	DMA_ControlTable sramTXDma80Seq[SRAMTXTASKS];

extern	void	delay1us(void);
extern	void	delay5us(void);
extern	void	delay10us(void);
extern	void	delay30us(void);
extern	void	delay100us(void);
extern	void	delay1ms(void);
extern	void	delay10ms(void);
extern	void	delay50ms(void);
extern	void	delay100ms(void);
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

void	sramAdrSet(uint adr);
void	readInstruction(void);
void	writeInstruction(void);
void	sramCSLow(void);
void	sramCSHi(void);
void	indexSRAMCheckWrite(void);
uchar	indexSRAMCheckRead(void);
void	sdAdrWrite(void);
void	sdAdrRead(void);
void	indexSRAMPointWrite(void);
void	indexSRAMPointRead(void);
void	indexSRAMDayTimeWrite(void);
void	indexSRAMDayTimeRead(void);
void	indexSRAMWrite(void);
void	indexSRAMRead(void);
void	SRAMClear(uint adr);
void	SRAMRead(uint adr);
void	SRAMALLClear(void);
void	SRAMALLRead(void);
//void	sramDMAWrite(void);
void	sramDMARead(void);
uchar	sramCompTest(void);
void	sramSave(void);
void	sramLoad(void);

void	sramAdrSet(uint adr)
{
	sramAdrHi = (uchar)(adr >> 16);								// Adr Hi
	sramAdrMid = (uchar)(adr >> 8);								// Adr Mid
	sramAdrLow = (uchar)(adr & 0xff);							// Adr Low
}

void	readInstruction(void)
{
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0x03;											// Read CMD
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrHi;										// Adr Hi
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrMid;										// Adr Mid
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrLow;										// Adr Low
}

void	writeInstruction(void)
{
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0x02;											// Write CMD
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrHi;										// Adr Hi
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrMid;										// Adr Mid
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrLow;										// Adr Low
}

void	sramCSLow(void)
{
	if(sramSel == 0)
	{
		SRAM1_CS_Low;											// CS Off
	} else
	{
		SRAM2_CS_Low;											// CS Off
	}
}

void	sramCSHi(void)
{
	if(sramSel == 0)
	{
		SRAM1_CS_High;											// CS On
	} else
	{
		SRAM2_CS_High;											// CS On
	}
}

void	indexSRAMCheckWrite(void)
{
	uint		cnt;

	sramCSLow();
	sramAdrSet(SRAM_INDEX_CHECK_ADRESS);
	delay1us();

	writeInstruction();

	for( cnt=0; cnt < SRAM_CHECK_COUNT; cnt++)
	{
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = sramCheck[cnt];								//
	}
	delay1us();
	sramCSHi();
}

uchar	indexSRAMCheckRead(void)
{
	uint		cnt;

	sramCSLow();
	sramAdrSet(SRAM_INDEX_CHECK_ADRESS);
	delay1us();

	readInstruction();

	delay1us();
	delay1us();
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								// Mode Write=>Read
	while (!(UCA1IFG&UCTXIFG));									// Dummy 1byte Read
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								//

	for( cnt=0; cnt < SRAM_CHECK_COUNT; cnt++)
	{
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = 0xff;
		while (!(UCA1IFG&UCRXIFG));								//
		if(sramCheck[cnt] != (uchar)UCA1RXBUF)
		{
			break;
		}
	}
	delay1us();
	sramCSHi();
	if(cnt == SRAM_CHECK_COUNT)
	{
		return OK;
	}
	return NG;
}

void	sdAdrWrite(void)
{
	ulong		Work;

	sramCSLow();
	sramAdrSet(SRAM_INDEX_POINTER_ADRESS);
	delay1us();

	writeInstruction();

	Work = sdMainCount;
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)((Work & 0xff00000000) >> 32);
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)((Work & 0x00ff000000) >> 24);
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)((Work & 0x0000ff0000) >> 16);
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)((Work & 0x000000ff00) >> 8);
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)(Work & 0x00000000ff);

	delay1us();
	sramCSHi();
}

void	sdAdrRead(void)
{
	ulong		Work;

	sramCSLow();
	sramAdrSet(SRAM_INDEX_POINTER_ADRESS);
	delay1us();

	readInstruction();

	delay1us();
	delay1us();
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								// Mode Write=>Read
	while (!(UCA1IFG&UCTXIFG));									// Dummy 1byte Read
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								//

	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work = (uchar)UCA1RXBUF;									//
	Work = Work << 8;											//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work |= (uchar)UCA1RXBUF;									//
	Work = Work << 8;											//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work |= (uchar)UCA1RXBUF;									//
	Work = Work << 8;											//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work |= (uchar)UCA1RXBUF;									//
	Work = Work << 8;											//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work |= (uchar)UCA1RXBUF;									//
	sdMainCount = Work;											//

	delay1us();
	sramCSHi();
}

void	indexSRAMPointWrite(void)
{
	uint		Work;

	sramCSLow();
	sramAdrSet(SRAM_INDEX_POINTER_ADRESS + 8);
	delay1us();

	writeInstruction();

	Work = indexDatePointer;
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)((Work & 0xff000000) >> 24);
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)((Work & 0x00ff0000) >> 16);
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)((Work & 0x0000ff00) >> 8);
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = (uchar)(Work & 0x000000ff);

	delay1us();
	sramCSHi();
}

void	indexSRAMPointRead(void)
{
	ulong		Work;

	sramCSLow();
	sramAdrSet(SRAM_INDEX_POINTER_ADRESS + 8);
	delay1us();

	readInstruction();

	delay1us();
	delay1us();
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								// Mode Write=>Read
	while (!(UCA1IFG&UCTXIFG));									// Dummy 1byte Read
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								//
//	while (!(UCA1IFG&UCTXIFG));									//
//	UCA1TXBUF = 0xff;											//
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//

	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work = (uchar)UCA1RXBUF;									//
	Work = Work << 8;											//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work |= (uchar)UCA1RXBUF;									//
	Work = Work << 8;											//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work |= (uchar)UCA1RXBUF;									//
	Work = Work << 8;											//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	Work |= (uchar)UCA1RXBUF;									//
	indexDatePointer = (uchar)Work;									//

	delay1us();
	sramCSHi();
}

void	indexSRAMDayTimeWrite(void)
{
	uint		cnt1, cnt2;

	sramCSLow();
	sramAdrSet(SRAM_DAYTIME_START_ADRESS);
	delay1us();

	writeInstruction();

	for( cnt1 = 0; cnt1 < 10; cnt1++)
	{
		for( cnt2 = 0; cnt2 < 8; cnt2++)
		{
			while (!(UCA1IFG&UCTXIFG));							//
			UCA1TXBUF = indexDate[cnt1][cnt2];					//
		}
	}
	delay1us();
	sramCSHi();
}

void	indexSRAMDayTimeRead(void)
{
	uint		cnt1, cnt2;

	sramCSLow();
	sramAdrSet(SRAM_DAYTIME_START_ADRESS);
	delay1us();

	readInstruction();

	delay1us();
	delay1us();
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								// Mode Write=>Read
	while (!(UCA1IFG&UCTXIFG));									// Dummy 1byte Read
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								//
//	while (!(UCA1IFG&UCTXIFG));									//
//	UCA1TXBUF = 0xff;											//
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//

	for( cnt1 = 0; cnt1 < 10; cnt1++)
	{
		for( cnt2 = 0; cnt2 < 8; cnt2++)
		{
			while (!(UCA1IFG&UCTXIFG));							//
			UCA1TXBUF = 0xff;
			while (!(UCA1IFG&UCRXIFG));							//
			indexDate[cnt1][cnt2] = (uchar)UCA1RXBUF;			//
		}
	}
	delay1us();
	sramCSHi();
}

void	indexSRAMWrite(void)
{
	uint		cnt;

	sramCSLow();
	sramAdrSet(SRAM_INDEX_START_ADRESS);
	delay1us();

	writeInstruction();

	for( cnt=0; cnt < MAX_DATA_INDEX; cnt++)
	{
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = (uchar)((indexAdrBuf[cnt] & 0xff000000) >> 24);	//
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = (uchar)((indexAdrBuf[cnt] & 0x00ff0000) >> 16);	//
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = (uchar)((indexAdrBuf[cnt] & 0x0000ff00) >> 8);	//
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = (uchar)(indexAdrBuf[cnt] & 0x000000ff);		//
	}
	delay1us();
	sramCSHi();
}

void	indexSRAMRead(void)
{
	uint		cnt;
	uint		Work;

	sramCSLow();
	sramAdrSet(SRAM_INDEX_START_ADRESS);
	delay1us();

	readInstruction();

	delay1us();
	delay1us();
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								// Mode Write=>Read
	while (!(UCA1IFG&UCTXIFG));									// Dummy 1byte Read
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								//
//	while (!(UCA1IFG&UCTXIFG));									//
//	UCA1TXBUF = 0xff;											//
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//

	for( cnt=0; cnt < MAX_DATA_INDEX; cnt++)
	{
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = 0xff;										//
		while (!(UCA1IFG&UCRXIFG));								//
		Work = (uchar)UCA1RXBUF;								//
		Work = Work << 8;										//
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = 0xff;										//
		while (!(UCA1IFG&UCRXIFG));								//
		Work |= (uchar)UCA1RXBUF;								//
		Work = Work << 8;										//
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = 0xff;										//
		while (!(UCA1IFG&UCRXIFG));								//
		Work |= (uchar)UCA1RXBUF;								//
		Work = Work << 8;										//
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = 0xff;										//
		while (!(UCA1IFG&UCRXIFG));								//
		Work |= (uchar)UCA1RXBUF;								//
		indexAdrBuf[cnt] = Work;								//
	}
	delay1us();
	sramCSHi();
}

void	SRAMClear(uint adr)
{
	uint		cnt;

	sramCSLow();
	sramAdrHi = (uchar)(adr >> 16);								// Adr Hi
	sramAdrMid = (uchar)(adr >> 8);								// Adr Mid
	sramAdrLow = (uchar)(adr & 0xff);							// Adr Low
	delay1us();

	writeInstruction();

	for( cnt=0; cnt < 0x800; cnt++)
	{
		while (!(UCA1IFG&UCTXIFG));								//
//		UCA1TXBUF = testData;
		UCA1TXBUF = 0;
	}
	delay1us();
	sramCSHi();
}

void	SRAMRead(uint adr)
{
#if 0
	uint		cnt;

	sramCSLow();
	sramAdrHi = (uchar)(adr >> 16);								// Adr Hi
	sramAdrMid = (uchar)(adr >> 8);								// Adr Mid
	sramAdrLow = (uchar)(adr & 0xff);							// Adr Low
	delay1us();

	readInstruction();

	delay1us();
	delay1us();
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								// Mode Write=>Read
	while (!(UCA1IFG&UCTXIFG));									// Dummy 1byte Read
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								//
//	while (!(UCA1IFG&UCTXIFG));									//
//	UCA1TXBUF = 0xff;											//
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//

	for( cnt=0; cnt < 0x800; cnt++)
	{
		UCA1TXBUF = 0xff;
		while (!(UCA1IFG&UCRXIFG));								//
		testBuf[cnt] = (uchar)UCA1RXBUF;						//
	}
	delay1us();
	sramCSHi();
#endif
}

void	SRAMALLClear(void)
{
	uint	cnt;

	for(cnt = 0; cnt < 0x100; cnt++)
	{
//		testData = cnt;
		SRAMClear(cnt * 0x800);
	}
}

void	SRAMALLRead(void)
{
	uint	cnt;

	for(cnt = 0; cnt < 0x100; cnt++)
	{
		SRAMRead(cnt * 0x800);
	}
}

#if 0
void	sramDMAWrite(void)
{
//	uint		cnt;

#if 1
	/*
	* Start SPI read
	*/
	writeInstADRBuf[0] = 0x02;
	writeInstADRBuf[1] = (uchar)(sramAdr >> 16);				// Adr Hi
	writeInstADRBuf[2] = (uchar)(sramAdr >> 8);					// Adr Mid
	writeInstADRBuf[3] = (uchar)(sramAdr & 0xff);				// Adr Low

	MAP_DMA_setChannelScatterGather(SRAMDMACHSPI,SRAMTXTASKS,(void*)&sramTXDma00Seq[0],0);

	MAP_DMA_clearInterruptFlag(SRAMDMACHSPI & 0x0F);

	/* Assigning/Enabling Interrupts */
	MAP_DMA_enableInterrupt(INT_DMA_INT3);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
	MAP_DMA_enableChannel(SRAMDMACHSPI);

	/*
	* The DMA channel3 is set to trigger off of theEUSCIB0RX IFG.  This
	* did not really happen but is forced manually to start the
	* scatter-gather sequence.  Because the setting is 'memory'
	* scatter-gather all triggers (transitions) from primary to
	* auxiliary actions will be automatic.  The auxiliary action triggers
	* are defined in the task list.
	*/
	EUSCI_A1_SPI->IFG |= EUSCI_A_IFG_RXIFG;
#else

//	MAP_SPI_clearInterruptFlag(EUSCI_A1_BASE,EUSCI_A_SPI_RECEIVE_INTERRUPT);
	MAP_DMA_setChannelScatterGather(SRAMDMACHSPI,SRAMTXTASKS,(void*)&sramTXDma00Seq[0],0);
	MAP_DMA_clearInterruptFlag(SRAMDMACHSPI & 0x0F);

	MAP_DMA_enableInterrupt(INT_DMA_INT3);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
	MAP_DMA_enableChannel(SRAMDMACH);

	writeSRAMStatus = ON;

	EUSCI_A1_SPI->IFG |= EUSCI_A_IFG_RXIFG;
#endif
}
#endif

void	sramDMARead(void)
{

#if 0
	sramAdrHi = (uchar)(sramReadAdr >> 16);						// Adr Hi
	sramAdrMid = (uchar)(sramReadAdr >> 8);						// Adr Mid
	sramAdrLow = (uchar)(sramReadAdr & 0xff);					// Adr Low
	delay1us();

//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0x03;											// Read CMD
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrHi;										// Adr Hi
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrMid;										// Adr Mid
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = sramAdrLow;										// Adr Low
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCTXIFG));									//
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//
//	while (!(UCA1IFG&UCTXIFG));									//
//	UCA1TXBUF = 0xff;											//
//	while (!(UCA1IFG&UCRXIFG));									//
//	dummyData = (uchar)UCA1RXBUF;								//
#else
	readInstADRBuf[0] = 0x03;
	readInstADRBuf[1] = (uchar)(sramReadAdr >> 16);				// Adr Hi
	readInstADRBuf[2] = (uchar)(sramReadAdr >> 8);				// Adr Mid
	readInstADRBuf[3] = (uchar)(sramReadAdr & 0xff);			// Adr Low
#endif

	MAP_SPI_clearInterruptFlag(EUSCI_A1_BASE,EUSCI_A_SPI_RECEIVE_INTERRUPT);
	MAP_DMA_setChannelScatterGather(SRAMDMACHSPI,SRAMRXTASKS00,(void*)&sramRXDma00Seq[0],0);
	MAP_DMA_clearInterruptFlag(SRAMDMACHSPI & 0x0F);

	MAP_DMA_enableInterrupt(INT_DMA_INT3);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
	MAP_DMA_enableChannel(SRAMDMACH);

	readSRAMStatus = ON;

	EUSCI_A1_SPI->IFG |= EUSCI_A_IFG_RXIFG;
}

#if 0
uchar	sramCompTest(uint work)
{

	if((work % 3) == 0)
	{
		work = sramSaveCount + work / 3 * 0x40;
		if(
		(sramReadBuf[3*0 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sramReadBuf[3*0 + 2] != (uchar)(work & 0x000000ff)) ||
		(sramReadBuf[3*1 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sramReadBuf[3*1 + 2] != (uchar)(work & 0x000000ff)) ||
		(sramReadBuf[3*2 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sramReadBuf[3*2 + 2] != (uchar)(work & 0x000000ff)) ||
		(sramReadBuf[3*3 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sramReadBuf[3*3 + 2] != (uchar)(work & 0x000000ff)) ||
		(sramReadBuf[3*4 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sramReadBuf[3*4 + 2] != (uchar)(work & 0x000000ff)) ||
		(sramReadBuf[3*5 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sramReadBuf[3*5 + 2] != (uchar)(work & 0x000000ff)) ||
		(sramReadBuf[3*6 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sramReadBuf[3*6 + 2] != (uchar)(work & 0x000000ff)) ||
		(sramReadBuf[3*7 + 1] != (uchar)((work & 0x00007f00) >> 8)) ||
		(sramReadBuf[3*7 + 2] != (uchar)(work & 0x000000ff)))
		{
			return 0xff;
		}
	}
	return 0;
}
#else
uchar	sramCompTest(void)
{
//	if(memcmp(sramWriteBuf, sramReadBuf, sizeof(sramWriteBuf)) != 0)
//	{
//		return 0xFF;
//	}
	return 0;
}
#endif

void	sramSave(void)
{
#if	0
	sramCSLow();

	writeInstADRBuf[0] = 0x02;
	writeInstADRBuf[1] = (uchar)(sramSaveCount >> 16);			// Adr Hi
	writeInstADRBuf[2] = (uchar)(sramSaveCount >> 8);			// Adr Mid
	writeInstADRBuf[3] = (uchar)(sramSaveCount & 0xff);			// Adr Low

/*
	* Start SPI read
	*/
	MAP_DMA_setChannelScatterGather(SRAMDMACHSPI,SRAMTXTASKS,(void*)&sramTXDma00Seq[0],0);

	MAP_DMA_clearInterruptFlag(SRAMDMACHSPI & 0x0F);

	/* Assigning/Enabling Interrupts */
	MAP_DMA_enableInterrupt(INT_DMA_INT3);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
	MAP_DMA_enableChannel(SRAMDMACHSPI);

	/*
	* The DMA channel3 is set to trigger off of theEUSCIB0RX IFG.  This
	* did not really happen but is forced manually to start the
	* scatter-gather sequence.  Because the setting is 'memory'
	* scatter-gather all triggers (transitions) from primary to
	* auxiliary actions will be automatic.  The auxiliary action triggers
	* are defined in the task list.
	*/
	writeSRAMDMAStatus = SRAMWRITEDMASTATUS01;
	EUSCI_A1_SPI->IFG |= EUSCI_A_IFG_RXIFG;

#else
	uint		cnt1;
//	uchar		dummy;
//	uchar		*buf;

	sramCSLow();
	sramAdrSet((uint)sramSaveCount);
	delay1us();

	writeInstruction();

	for( cnt1 = 0; cnt1 < TESTBLOCK * TESTWCNT; cnt1++)
	{
		while (!(UCA1IFG&UCTXIFG));								//
		UCA1TXBUF = sramWriteBuf[cnt1];							//
	}
	delay1us();
	sramCSHi();
#endif
}

void	sramLoad(void)
{
#if 0
//ledD1On();
	sramCSLow();

	readInstADRBuf[0] = 0x03;
	readInstADRBuf[1] = (uchar)(sramLoadCount >> 16);			// Adr Hi
	readInstADRBuf[2] = (uchar)(sramLoadCount >> 8);			// Adr Mid
	readInstADRBuf[3] = (uchar)(sramLoadCount & 0xff);			// Adr Low

	MAP_SPI_clearInterruptFlag(EUSCI_A1_BASE,EUSCI_A_SPI_RECEIVE_INTERRUPT);
	MAP_DMA_setChannelScatterGather(SRAMDMACHSPI,SRAMRXTASKS00,(void*)&sramRXDma00Seq[0],0);
	MAP_DMA_clearInterruptFlag(SRAMDMACHSPI & 0x0F);

	MAP_DMA_enableInterrupt(INT_DMA_INT3);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
	MAP_DMA_enableChannel(SRAMDMACH);

	readSRAMDMAStatus = SRAMREADDMASTATUS01;
	EUSCI_A1_SPI->IFG |= EUSCI_A_IFG_RXIFG;
#else
	uint		cnt1;
//	uchar		rep = 0;

	sramCSLow();
	sramAdrSet((uint)sramLoadCount);
	delay1us();

	readInstruction();

	delay1us();
	delay1us();
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								// Mode Write=>Read
	while (!(UCA1IFG&UCTXIFG));									// Dummy 1byte Read
	UCA1TXBUF = 0xff;											//
	while (!(UCA1IFG&UCRXIFG));									//
	dummyData = (uchar)UCA1RXBUF;								//
//	for( cnt1 = 0; cnt1 < TESTBLOCK * TESTWCNT; cnt1++)
	for( cnt1 = 0; cnt1 < SRAM_LOAD_LENGTH; cnt1++)
	{
		UCA1TXBUF = 0xff;										//
		while (!(UCA1IFG&UCRXIFG));								//
		sramReadBuf[cnt1] = (uchar)UCA1RXBUF;					//
	}
//	rep = sramCompTest();
//	if(rep != 0)
//	{
//		return	rep;
//	}
	delay1us();
	sramCSHi();
//	return	rep;
#endif
}


/****************************************
 *	adc.c								*
 ****************************************/

#include	<string.h>
#include	"msp.h"
#include	"define.h"
#include	"extern.h"

extern	void	delay1us(void);
extern	void	delay10us(void);
extern	void	delay30us(void);
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

void	adcHeaderChk(void);
uint	adcSVRead(uchar data1, uchar data2);
void	adcSVWrite(uchar data1, uchar data2);
void	adcRead(void);

void	adcHeaderChk(void)
{
	if(((adChHeader.Ch0 & 0xf0) != ADHEAD0) || ((adChHeader.Ch1 & 0xf0) != ADHEAD1) || ((adChHeader.Ch2 & 0xf0) != ADHEAD2) || ((adChHeader.Ch3 & 0xf0) != ADHEAD3)
	|| ((adChHeader.Ch4 & 0xf0) != ADHEAD4) || ((adChHeader.Ch5 & 0xf0) != ADHEAD5) || ((adChHeader.Ch6 & 0xf0) != ADHEAD6) || ((adChHeader.Ch7 & 0xf0) != ADHEAD7))
	{
		adChHeader.Ch0ERR = adChHeader.Ch0;
		adChHeader.Ch1ERR = adChHeader.Ch1;
		adChHeader.Ch2ERR = adChHeader.Ch2;
		adChHeader.Ch3ERR = adChHeader.Ch3;
		adChHeader.Ch4ERR = adChHeader.Ch4;
		adChHeader.Ch5ERR = adChHeader.Ch5;
		adChHeader.Ch6ERR = adChHeader.Ch6;
		adChHeader.Ch7ERR = adChHeader.Ch7;
		adErrStatus = AD_HEAD_ERR;
	} else
	{
		adErrStatus = NO_ERR;
	}
}

uint	adcSVRead(uchar data1, uchar data2)
{
	long	rep = 0;

//	data1 = data1 & ~0x80;										// WriteBit Clear

#if 1
	P6OUT &= ~BIT2;
	delay1us();
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = data1;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep = (long)UCB2RXBUF;
	rep <<= 8;
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = data2;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep |= (long)UCB2RXBUF;
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	delay1us();
	P6OUT |= BIT2;												//
	delay1us();
	return	rep;
#else
	P6OUT &= ~BIT2;
	delay10us();
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = data1;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep = (long)UCB2RXBUF;
	rep <<= 8;
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = data2;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep |= (long)UCB2RXBUF;
	rep <<= 8;
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = 0x00;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep |= (long)UCB2RXBUF;
	delay10us();
	P6OUT |= BIT2;												//
	delay30us();
	return	rep;
#endif
}

void	adcSVWrite(uchar data1, uchar data2)
{
	long	rep;

//	data1 = data1 | 0x80;												// WriteBit Set

#if 1
	P6OUT &= ~BIT2;
	delay1us();
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = data1;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep = (long)UCB2RXBUF;
	rep <<= 8;
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = data2;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep |= (long)UCB2RXBUF;
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	delay1us();
	P6OUT |= BIT2;												//
	delay1us();
//	return	rep;
#else
	P6OUT &= ~BIT2;
	delay10us();
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = data1;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep = (long)UCB2RXBUF;
	rep <<= 8;
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = data2;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep |= (long)UCB2RXBUF;
	rep <<= 8;
	while (!(UCB2IFG&UCTXIFG));									// USCI_B2 TX buffer ready?
	UCB2TXBUF = 0x00;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	rep |= (long)UCB2RXBUF;
	delay10us();
	P6OUT |= BIT2;												//
	delay30us();
	return	rep;
#endif
}

void	adcRead(void)
{

	P6OUT &= ~BIT2;

// Ch0
	uchar*	adBufPoint = &adBuf[adBufAdr];

	memset(&adBuf[adBufAdr], 0, AD_DATA_COUNT);
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	adChHeader.Ch0 = (long)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;

// Ch1
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	adChHeader.Ch1 = (long)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;

// Ch2
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	adChHeader.Ch2 = (long)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;

// Ch3
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	adChHeader.Ch3 = (long)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;

// Ch4
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	adChHeader.Ch4 = (long)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;

// Ch5
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	adChHeader.Ch5 = (long)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;

// Ch6
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	adChHeader.Ch6 = (long)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;

// Ch7
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	adChHeader.Ch7 = (long)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;
	UCB2TXBUF = OUTCLK;
	while (!(UCB2IFG&UCRXIFG));									// USCI_B2 RX buffer ready?
	*adBufPoint++ = (uchar)UCB2RXBUF;

	P6OUT |= BIT2;												//

#if 1
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


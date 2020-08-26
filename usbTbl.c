/****************************************
 *	datatbl.c								*
 ****************************************/

#include	<Lib/driverlib.h>
#include	<string.h>
#include	"msp.h"
#include	"define.h"
#include	"extern.h"

extern	void	sdAdrWrite(void);
extern	void	sdAdrRead(void);
extern	uchar	sdBlockRead(long blocknum);
extern	void	indexSRAMPointWrite(void);
extern	void	indexSRAMPointRead(void);
extern	void	indexSRAMDayTimeWrite(void);
extern	void	indexSRAMDayTimeRead(void);
extern	void	indexSRAMWrite(void);
extern	void	indexSRAMRead(void);
extern	char	ADPCM_Encoder(signed int Input);
extern	signed	int	ADPCM_Decoder(char	temp);
extern	void	adpcm_main(void);
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
extern	void	delay1us(void);
extern	void	delay1s(void);

extern	DMA_ControlTable usbTXDmaData00Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma00Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma02Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma04Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma08Seq[USBTXTASKS];
extern	DMA_ControlTable usbTXDma16Seq[USBTXTASKS];

void	usbLFSerialOut(void);
void	usbCompData(void);
void	usbSerialAdrOut(uchar *adr, ulong cntValue);
void	usbSerialCharaOut(uchar chara, ulong cntValue);
void	usbDataSend(void);
void	usbDMADataSendSet(void);
void	usbDMASendSet(void);
void	usbDMAHeaderInit(void);
void	usbDMAHeaderSet(void);
void	usbDMASend(void);
void	usbSend(void);

void	usbLFSerialOut(void)
{
	P1->OUT &= ~BIT4;
	UCB0TXBUF = (uint)0x0A;
	while (!(UCB0IFG&UCTXIFG));
//	delay1us();
//	P1->OUT |= BIT4;

}

void	usbCompData(void)
{
	ulong	tempData;
//	ulong	tempData0,tempData1,tempData2,tempData3;
//	ulong	tempData4,tempData5,tempData6,tempData7;

#if 1
	tempData = ((uint)usbWriteBuf[1]) << 8;
	tempData += usbWriteBuf[2];
	if(testUsbData != tempData)
	{
		usbErrStatus = USB_ERR;
	}
	tempData = ((uint)usbWriteBuf[4]) << 8;
	tempData += usbWriteBuf[5];
	if(testUsbData != tempData)
	{
		usbErrStatus = USB_ERR;
	}
	tempData = ((uint)usbWriteBuf[7]) << 8;
	tempData += usbWriteBuf[8];
	if(testUsbData != tempData)
	{
		usbErrStatus = USB_ERR;
	}
	tempData = ((uint)usbWriteBuf[10]) << 8;
	tempData += usbWriteBuf[11];
	if(testUsbData != tempData)
	{
		usbErrStatus = USB_ERR;
	}
	tempData = ((uint)usbWriteBuf[13]) << 8;
	tempData += usbWriteBuf[14];
	if(testUsbData != tempData)
	{
		usbErrStatus = USB_ERR;
	}
	tempData = ((uint)usbWriteBuf[16]) << 8;
	tempData += usbWriteBuf[17];
	if(testUsbData != tempData)
	{
		usbErrStatus = USB_ERR;
	}
	tempData = ((uint)usbWriteBuf[19]) << 8;
	tempData += usbWriteBuf[20];
	if(testUsbData != tempData)
	{
		usbErrStatus = USB_ERR;
	}
	tempData = ((uint)usbWriteBuf[22]) << 8;
	tempData += usbWriteBuf[23];
	if(testUsbData != tempData)
	{
		usbErrStatus = USB_ERR;
	}
//		if((testUsbData != tempData0) || (testUsbData != tempData1) ||
//		(testUsbData != tempData2) || (testUsbData != tempData3) ||
//		(testUsbData != tempData4) || (testUsbData != tempData5) ||
//		(testUsbData != tempData6) || (testUsbData != tempData7))
//		{
//			usbErrStatus = USB_ERR;
//		}
	testUsbData += 8;
	if(testUsbData > 0x007fff)
	{
		testUsbData = 0;
	}
	if(usbErrStatus != 0)
	{
//		ledD2On();
	}
#else
	uint	cnt;
	uint	ptr = 0;

	for(cnt = 0; cnt < 8; cnt++)
	{
		tempData = usbWriteBuf[ptr + 2];
		if(testUsbData != tempData)
		{
			usbErrStatus = USB_ERR;
		}
		tempData = usbWriteBuf[ptr + 5];
		if(testUsbData != tempData)
		{
			usbErrStatus = USB_ERR;
		}
		tempData = usbWriteBuf[ptr + 8];
		if(testUsbData != tempData)
		{
			usbErrStatus = USB_ERR;
		}
		tempData = usbWriteBuf[ptr + 11];
		if(testUsbData != tempData)
		{
			usbErrStatus = USB_ERR;
		}
		tempData = usbWriteBuf[ptr + 14];
		if(testUsbData != tempData)
		{
			usbErrStatus = USB_ERR;
		}
		tempData = usbWriteBuf[ptr + 17];
		if(testUsbData != tempData)
		{
			usbErrStatus = USB_ERR;
		}
		tempData = usbWriteBuf[ptr + 20];
		if(testUsbData != tempData)
		{
			usbErrStatus = USB_ERR;
		}
		tempData = usbWriteBuf[ptr + 23];
		if(testUsbData != tempData)
		{
			usbErrStatus = USB_ERR;
		}
		ptr += 24;
		testUsbData += 1;
		if(testUsbData > 0x0000ff)
		{
			testUsbData = 0;
		}
	}
	if(usbErrStatus != 0)
	{
//		ledD2On();
	}
#endif
}

void	usbSerialAdrOut(uchar *adr, ulong cntValue)
{
	ulong	cnt;

	P1->OUT &= ~BIT4;
	for(cnt = 0; cnt < cntValue; cnt++)
	{
		UCB0TXBUF = (uint)adr[cnt];
		while (!(UCB0IFG&UCTXIFG));
	}
	delay1us();
	P1->OUT |= BIT4;
}

void	usbSerialCharaOut(uchar chara, ulong cntValue)
{
	ulong	cnt;

	P1->OUT &= ~BIT4;
	for(cnt = 0; cnt < cntValue; cnt++)
	{
		UCB0TXBUF = (uint)chara;
		while (!(UCB0IFG&UCTXIFG));
	}
	delay1us();
	P1->OUT |= BIT4;
}

void	usbDataSend(void)
{
	//recStartTime		0x0000
//	usbSerialAdrOut((uchar*) recStartTime, sizeof(recStartTime));

	listWork[0] = indexDate[dataNo -1][0];						// YY
	listWork[1] = indexDate[dataNo -1][1];						// YY
	listWork[2] = indexDate[dataNo -1][2];						// MM
	listWork[3] = indexDate[dataNo -1][3];						// DD
	listWork[4] = indexDate[dataNo -1][4];						// hh
	listWork[5] = indexDate[dataNo -1][5];						// mm
	listWork[6] = 0x00;
	listWork[7] = 0x00;

	usbSerialAdrOut((uchar*) listWork, 8);

	//recStopTime		0x0008
//	usbSerialAdrOut((uchar*) recStopTime, sizeof(recStopTime));

	listWork[0] = indexDate[dataNo -1][0];						// YY
	listWork[1] = indexDate[dataNo -1][1];						// YY
	listWork[2] = indexDate[dataNo -1][2];						// MM
	listWork[3] = indexDate[dataNo -1][3];						// DD
	listWork[4] = indexDate[dataNo -1][6];						// hh
	listWork[5] = indexDate[dataNo -1][7];						// mm
	listWork[6] = 0x00;
	listWork[7] = 0x00;

	usbSerialAdrOut((uchar*) listWork, 8);

	//sampCount			0x0010
	usbSerialAdrOut((uchar*) sampCount, sizeof(sampCount));

	//unitSerialNo		0x0020
	if(unitType == MASTER)
	{
		usbSerialAdrOut((uchar*) unitSerialNoMaster, sizeof(unitSerialNoMaster));
	} else
	{
		usbSerialAdrOut((uchar*) unitSerialNoSlave, sizeof(unitSerialNoSlave));
	}

	//verNo				0x0040
	usbSerialAdrOut((uchar*) verNo, sizeof(verNo));

	//sdCapa			0x0050
	usbSerialAdrOut((uchar*) sdCapa, sizeof(sdCapa));

	//recordPoint		0x0060
	usbSerialAdrOut((uchar*) recordPoint, sizeof(recordPoint));

	//temperature		0x0070
	usbSerialAdrOut((uchar*) temperature, sizeof(temperature));

	//humidity			0x0078
	usbSerialAdrOut((uchar*) humidity, sizeof(humidity));

	//statuCode			0x0080
	usbSerialAdrOut((uchar*) statuCode, sizeof(statuCode));

	//channelEnable		0x0090
	usbSerialAdrOut((uchar*) channelEnable, sizeof(channelEnable));

	//decimation		0x0098
	usbSerialAdrOut((uchar*) decimation, sizeof(decimation));

	//adPGA				0x00A0
	usbSerialAdrOut((uchar*) adPGA, sizeof(adPGA));

	//recCapaSet		0x00A8
	usbSerialAdrOut((uchar*) recCapaSet, sizeof(recCapaSet));

	//modeSet			0x00B0
	usbSerialAdrOut((uchar*) modeSet, sizeof(modeSet));

	//recTimeSet		0x00C0
	usbSerialAdrOut((uchar*) recTimeSet, sizeof(recTimeSet));

	//preRecTimeSet		0x00C8
	usbSerialAdrOut((uchar*) preRecTimeSet, sizeof(preRecTimeSet));

	//turiggerSet		0x00D0
	usbSerialAdrOut((uchar*) turiggerSet, sizeof(turiggerSet));

	//turiggerRelease	0x0100
	usbSerialAdrOut((uchar*) turiggerRelease, sizeof(turiggerRelease));

	//adOffset			0x0130
	usbSerialAdrOut((uchar*) adOffset, sizeof(adOffset));

	//adGain			0x0160
	usbSerialAdrOut((uchar*) adGain, sizeof(adGain));

	//reserve			0x0190
	usbSerialCharaOut((uchar) NULL, 0x70);

	cmdDataTrans = ON;

}

void	usbDMADataSendSet(void)
{
	MAP_DMA_setChannelScatterGather(USBDMACHSPI,USBTXTASKS,(void*)&usbTXDmaData00Seq[0],0);

	MAP_DMA_clearInterruptFlag(USBDMACHSPI & 0x0F);

	MAP_DMA_enableInterrupt(INT_DMA_INT3);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
	MAP_DMA_enableChannel(USBDMACH);
	P1->OUT &= ~BIT4;

	EUSCI_B0_SPI->IFG |= EUSCI_B_IFG_RXIFG;
}

void	usbDMASendSet(void)
{
	switch(reSampleSetValue)
	{
		case ADRESAMP00:
			MAP_DMA_setChannelScatterGather(USBDMACHSPI,USBTXTASKS,(void*)&usbTXDma00Seq[0],0);
			break;
		case ADRESAMP02:
			MAP_DMA_setChannelScatterGather(USBDMACHSPI,USBTXTASKS,(void*)&usbTXDma02Seq[0],0);
			break;
		case ADRESAMP04:
			MAP_DMA_setChannelScatterGather(USBDMACHSPI,USBTXTASKS,(void*)&usbTXDma04Seq[0],0);
			break;
		case ADRESAMP08:
			MAP_DMA_setChannelScatterGather(USBDMACHSPI,USBTXTASKS,(void*)&usbTXDma08Seq[0],0);
			break;
		case ADRESAMP16:
			MAP_DMA_setChannelScatterGather(USBDMACHSPI,USBTXTASKS,(void*)&usbTXDma16Seq[0],0);
			break;
	}

	MAP_DMA_clearInterruptFlag(USBDMACHSPI & 0x0F);

	MAP_DMA_enableInterrupt(INT_DMA_INT3);
	MAP_Interrupt_enableInterrupt(INT_DMA_INT3);
	MAP_DMA_enableChannel(USBDMACH);
	P1->OUT &= ~BIT4;

	EUSCI_B0_SPI->IFG |= EUSCI_B_IFG_RXIFG;
}

void	usbDMAHeaderInit(void)
{
	//recStartTime		0x0000
	memcpy(&usbHeaderBuf[0x0000], recStartTime, sizeof(recStartTime));
	//recStopTime		0x0008
	memcpy(&usbHeaderBuf[0x0008], recStopTime, sizeof(recStopTime));
	//sampCount			0x0010
	memcpy(&usbHeaderBuf[0x0010], sampCount, sizeof(sampCount));
	//unitSerialNo		0x0020
	if(unitType == MASTER)
	{
		memcpy(&usbHeaderBuf[0x0020], unitSerialNoMaster, sizeof(unitSerialNoMaster));
	} else
	{
		memcpy(&usbHeaderBuf[0x0020], unitSerialNoSlave, sizeof(unitSerialNoSlave));
	}
	//verNo				0x0040
	memcpy(&usbHeaderBuf[0x0040], verNo, sizeof(verNo));
	//sdCapa			0x0050
	memcpy(&usbHeaderBuf[0x0050], sdCapa, sizeof(sdCapa));
	//recordPoint		0x0060
	memcpy(&usbHeaderBuf[0x0060], recordPoint, sizeof(recordPoint));
	//temperature		0x0070
	memcpy(&usbHeaderBuf[0x0070], temperature, sizeof(temperature));
	//humidity			0x0078
	memcpy(&usbHeaderBuf[0x0078], humidity, sizeof(humidity));
	//statuCode			0x0080
	memcpy(&usbHeaderBuf[0x0080], statuCode, sizeof(statuCode));
	//channelEnable		0x0090
	memcpy(&usbHeaderBuf[0x0090], channelEnable, sizeof(channelEnable));
	//decimation		0x0098
	memcpy(&usbHeaderBuf[0x0098], decimation, sizeof(decimation));
	//adPGA				0x00A0
	memcpy(&usbHeaderBuf[0x00A0], adPGA, sizeof(adPGA));
	//recCapaSet		0x00A8
	memcpy(&usbHeaderBuf[0x00A8], recCapaSet, sizeof(recCapaSet));
	//modeSet			0x00B0
	memcpy(&usbHeaderBuf[0x00B0], modeSet, sizeof(modeSet));
	//recTimeSet		0x00C0
	memcpy(&usbHeaderBuf[0x00C0], recTimeSet, sizeof(recTimeSet));
	//preRecTimeSet		0x00C8
	memcpy(&usbHeaderBuf[0x00C8], preRecTimeSet, sizeof(preRecTimeSet));
	//turiggerSet		0x00D0
	memcpy(&usbHeaderBuf[0x00D0], turiggerSet, sizeof(turiggerSet));
	//turiggerRelease	0x0100
	memcpy(&usbHeaderBuf[0x0100], turiggerRelease, sizeof(turiggerRelease));
	//adOffset			0x0130
	memcpy(&usbHeaderBuf[0x0130], adOffset, sizeof(adOffset));
	//adGain			0x0160
	memcpy(&usbHeaderBuf[0x0160], adGain, sizeof(adGain));
	//reserve			0x0190
//	memcpy(&usbHeaderBuf[0x0190], reserve, sizeof(reserve));
}

void	usbDMAHeaderSet(void)
{
	//recStartTime		0x0000
//	memcpy(&usbHeaderBuf[0x0000], recStartTime, sizeof(recStartTime));

	usbHeaderBuf[0x0000] = (dateTime.year & 0xff00) >> 8;		// YY
	usbHeaderBuf[0x0001] = dateTime.year & 0xff;				// YY
	usbHeaderBuf[0x0002] = dateTime.month & 0xff; 				// MM
	usbHeaderBuf[0x0003] = dateTime.day & 0xff;				// DD
	usbHeaderBuf[0x0004] = dateTime.hours & 0xff; 				// hh
	usbHeaderBuf[0x0005] = dateTime.minutes & 0xff;  			// mm
	usbHeaderBuf[0x0006] = dateTime.seconds & 0xff;			// ss

	//recStopTime		0x0008
	memcpy(&usbHeaderBuf[0x0008], recStopTime, sizeof(recStopTime));
	//sampCount			0x0010
	memcpy(&usbHeaderBuf[0x0010], sampCount, sizeof(sampCount));
	//unitSerialNo		0x0020
	if(unitType == MASTER)
	{
		memcpy(&usbHeaderBuf[0x0020], unitSerialNoMaster, sizeof(unitSerialNoMaster));
	} else
	{
		memcpy(&usbHeaderBuf[0x0020], unitSerialNoSlave, sizeof(unitSerialNoSlave));
	}
	//verNo				0x0040
	memcpy(&usbHeaderBuf[0x0040], verNo, sizeof(verNo));
	//sdCapa			0x0050
	memcpy(&usbHeaderBuf[0x0050], sdCapa, sizeof(sdCapa));
	//recordPoint		0x0060
//	memcpy(&usbHeaderBuf[0x0060], recordPoint, sizeof(recordPoint));

	usbHeaderBuf[0x0060] = (recPoint.lat_h & 0xff00) >> 8;		// 
	usbHeaderBuf[0x0061] = recPoint.lat_h & 0xff;				// 
	usbHeaderBuf[0x0062] = (recPoint.lat_l & 0xff00) >> 8;		// 
	usbHeaderBuf[0x0063] = recPoint.lat_l & 0xff;				// 
	usbHeaderBuf[0x0064] = (recPoint.lng_h & 0xff00) >> 8;		// 
	usbHeaderBuf[0x0065] = recPoint.lng_h & 0xff;  			// 
	usbHeaderBuf[0x0066] = (recPoint.lng_l & 0xff00) >> 8;		// 
	usbHeaderBuf[0x0067] = recPoint.lng_l & 0xff;  			// 
	usbHeaderBuf[0x0068] = (recPoint.alt & 0xff00) >> 8;		// 
	usbHeaderBuf[0x0069] = recPoint.alt & 0xff;  				// 

	//temperature		0x0070
	memcpy(&usbHeaderBuf[0x0070], temperature, sizeof(temperature));
	//humidity			0x0078
	memcpy(&usbHeaderBuf[0x0078], humidity, sizeof(humidity));
	//statuCode			0x0080
	memcpy(&usbHeaderBuf[0x0080], statuCode, sizeof(statuCode));
	//channelEnable		0x0090
	memcpy(&usbHeaderBuf[0x0090], channelEnable, sizeof(channelEnable));
	//decimation		0x0098
	memcpy(&usbHeaderBuf[0x0098], decimation, sizeof(decimation));
	//adPGA				0x00A0
	memcpy(&usbHeaderBuf[0x00A0], adPGA, sizeof(adPGA));
	//recCapaSet		0x00A8
	memcpy(&usbHeaderBuf[0x00A8], recCapaSet, sizeof(recCapaSet));
	//modeSet			0x00B0
	memcpy(&usbHeaderBuf[0x00B0], modeSet, sizeof(modeSet));
	//recTimeSet		0x00C0
	memcpy(&usbHeaderBuf[0x00C0], recTimeSet, sizeof(recTimeSet));
	//preRecTimeSet		0x00C8
	memcpy(&usbHeaderBuf[0x00C8], preRecTimeSet, sizeof(preRecTimeSet));
	//turiggerSet		0x00D0
	memcpy(&usbHeaderBuf[0x00D0], turiggerSet, sizeof(turiggerSet));
	//turiggerRelease	0x0100
	memcpy(&usbHeaderBuf[0x0100], turiggerRelease, sizeof(turiggerRelease));
	//adOffset			0x0130
	memcpy(&usbHeaderBuf[0x0130], adOffset, sizeof(adOffset));
	//adGain			0x0160
	memcpy(&usbHeaderBuf[0x0160], adGain, sizeof(adGain));
	//reserve			0x0190
//	memcpy(&usbHeaderBuf[0x0190], reserve, sizeof(reserve));
}



void	usbDMASend(void)
{
	if(usbStart == ON)
	{
		if(dataStart == ON)
		{
//			ledD4OnOff();
			if(syncWaitFlag == OFF)
			{
				if(dataSendStep == STEP0)
				{
//					P7->OUT &= ~BIT5;
					usbDMAHeaderSet();
					usbSerialAdrOut((uchar*) usbHeaderBuf, sizeof(usbHeaderBuf));
					dataPakeCnt = 0;
					dataSendStep = STEP1;
				} else if(dataSendStep == STEP1)
				{
					usbEndAdr = adBufAdr;
					if(usbStartAdr > usbEndAdr)
					{
						usbEndAdr += AD_BUF_LENGTH;
					}
					if((usbEndAdr - usbStartAdr) >= usbWriteCountSV)
					{
						if((usbEndAdr - usbStartAdr) >= 200)
						{
							__no_operation();
						}
#if	0
						adpcm_main();
#else
#if	1
						memcpy(usbWriteBuf, (adBuf + usbStartAdr),usbWriteCountSV);
#if DEBUG
						usbCompData();
#endif
#else
						memset(usbWriteBuf, 0x00, usbWriteCountSV);
#endif
#endif
						if(dataPakeCnt == 0)								//sample			0x0200
						{
							usbLFSerialOut();
						}
						usbDMASendSet();
//						usbSerialAdrOut((uchar*) usbWriteBuf, sizeof(usbWriteBuf));
						dataPakeCnt++;
						if(dataPakeCnt == reSampleLFSetValue)
						{
							dataPakeCnt = 0;
						}

						usbStartAdr += usbWriteCountSV;
						if(usbStartAdr >= AD_BUF_LENGTH)
						{
							usbStartAdr -= AD_BUF_LENGTH;
						}
					}
				}
			}
		}
	}
}

void	usbSend(void)
{
	if(usbStart == ON)
	{
		if(dataStart == ON)
		{
			if(syncWaitFlag == OFF)
			{
				if(dataSendStep == STEP0)
				{
					//recStartTime		0x0000
					usbSerialAdrOut((uchar*) recStartTime, sizeof(recStartTime));

					//recStopTime		0x0008
					usbSerialAdrOut((uchar*) recStopTime, sizeof(recStopTime));

					//sampCount			0x0010
					usbSerialAdrOut((uchar*) sampCount, sizeof(sampCount));

					if(unitType == MASTER)
					{
						usbSerialAdrOut((uchar*) unitSerialNoMaster, sizeof(unitSerialNoMaster));
					} else
					{
						usbSerialAdrOut((uchar*) unitSerialNoSlave, sizeof(unitSerialNoSlave));
					}

					//verNo				0x0040
					usbSerialAdrOut((uchar*) verNo, sizeof(verNo));

					//sdCapa			0x0050
					usbSerialAdrOut((uchar*) sdCapa, sizeof(sdCapa));

					//recordPoint		0x0060
					usbSerialAdrOut((uchar*) recordPoint, sizeof(recordPoint));

					//temperature		0x0070
					usbSerialAdrOut((uchar*) temperature, sizeof(temperature));

					//humidity			0x0078
					usbSerialAdrOut((uchar*) humidity, sizeof(humidity));

					//statuCode			0x0080
					usbSerialAdrOut((uchar*) statuCode, sizeof(statuCode));

					//channelEnable		0x0090
					usbSerialAdrOut((uchar*) channelEnable, sizeof(channelEnable));

					//decimation		0x0098
					usbSerialAdrOut((uchar*) decimation, sizeof(decimation));

					//adPGA				0x00A0
					usbSerialAdrOut((uchar*) adPGA, sizeof(adPGA));

					//recCapaSet		0x00A8
					usbSerialAdrOut((uchar*) recCapaSet, sizeof(recCapaSet));

					//modeSet			0x00B0
					usbSerialAdrOut((uchar*) modeSet, sizeof(modeSet));

					//recTimeSet		0x00C0
					usbSerialAdrOut((uchar*) recTimeSet, sizeof(recTimeSet));

					//preRecTimeSet		0x00C8
					usbSerialAdrOut((uchar*) preRecTimeSet, sizeof(preRecTimeSet));

					//turiggerSet		0x00D0
					usbSerialAdrOut((uchar*) turiggerSet, sizeof(turiggerSet));

					//turiggerRelease	0x0100
					usbSerialAdrOut((uchar*) turiggerRelease, sizeof(turiggerRelease));

					//adOffset			0x0130
					usbSerialAdrOut((uchar*) adOffset, sizeof(adOffset));

					//adGain			0x0160
					usbSerialAdrOut((uchar*) adGain, sizeof(adGain));

					//reserve			0x0190
//					usbSerialAdrOut((uchar*) reserve, sizeof(reserve));
					usbSerialCharaOut((uchar) NULL, 0x70);

					dataPakeCnt = 0;
					dataSendStep = STEP1;
				} else if(dataSendStep == STEP1)
				{
					if(sramLoadOK == ON)
					{
//						memcpy(usbWriteBuf, &sramReadBuf[usbStartCount],usbWriteCountSV);
						if(dataPakeCnt == 0)								//sample			0x0200
						{
							usbLFSerialOut();
						}
						usbSerialAdrOut((uchar*) usbWriteBuf, sizeof(usbWriteBuf));
						dataPakeCnt++;
						if(dataPakeCnt == reSampleLFSetValue)
						{
							dataPakeCnt = 0;
						}
						usbStartCount += usbWriteCountSV;
						if(usbStartCount >= SRAM_READ_BUF_LENGTH)
						{
							usbStartCount = 0;
							sramLoadOK = OFF;
						}
					}
				}
			}
		}
	}
}


/****************************************
 *	sub.c								*
 ****************************************/

#include	<string.h>
#include	"msp.h"
#include	"define.h"
#include	"extern.h"

extern	void	sramCSLow(void);
extern	void	sramCSHi(void);
extern	void	indexSRAMCheckWrite(void);
extern	uchar	indexSRAMCheckRead(void);
extern	void	sdAdrWrite(void);
extern	void	sdAdrRead(void);
extern	uchar	sdBlockRead(long blocknum);
extern	void	SRAMClear(uint adr);

extern	void	usbSerialAdrOut(uchar *adr, ulong cntValue);
extern	void	usbSerialCharaOut(uchar chara, ulong cntValue);
extern	void	usbDataSend(void);
extern	void	usbDMADataSendSet(void);
extern	void	error(void);
extern	void	delay1us(void);
extern	void	rtcSet(void);
extern	void	rtcGet(void);
extern	void	ledD4Off(void);
extern	void	ledD4On(void);
extern	void	ledD4OnOff(void);
extern	void	Timer1Set(uint value);
extern	void	Timer2Set(ulong value);							// Command Time Set

void	crLFSend(void);
void	statusSend(void);
uchar	dayTimeRCVSet(void);
void	listTest(void);
void	listTestDummy(void);
void	dataCommand(void);
void	listCommand(void);
uchar	paraSetCommand(void);
void	paraGetCommand(void);
void	resetCommand(void);
void	stopCommand(void);
void	startCommand(void);
void	NGStatusSend(void);
void	OKStatusSend(void);
//void	NGSend(void);
//void	OKSend(void);
void	replySend(uchar rep);
void	commandDataTransSet(void);
void	commandDataTrans(void);
void	commandCheck(void);
void	commandSelect(void);

void	crLFSend(void)
{
	UCB0TXBUF = CR;
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	UCB0TXBUF = LF;
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
}

#if 0
void	statusSend(void)
{
	P1->OUT &= ~BIT4;
	UCB0TXBUF = 'O';
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	UCB0TXBUF = 'K';
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	UCB0TXBUF = SP;
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	UCB0TXBUF = statusCode;
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	crLFSend();
	delay1us();
	P1->OUT |= BIT4;
}
#else
void	statusSend(void)
{
	P1->OUT &= ~BIT4;

	if(sdErrStatus != NO_ERR)
	{
		statusCode = sdErrStatus;
		sdErrStatus = NO_ERR;
	} else if(adErrStatus != NO_ERR)
	{
		statusCode = adErrStatus;
		adErrStatus = NO_ERR;
	} else if(usbErrStatus != NO_ERR)
	{
		statusCode = usbErrStatus;
		usbErrStatus = NO_ERR;
	}
	UCB0TXBUF = (uchar)((statusCode & 0xff00) >> 8);
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	UCB0TXBUF = (uchar)(statusCode & 0x00ff);
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	crLFSend();
	delay1us();
	P1->OUT |= BIT4;
}
#endif

uchar	dayTimeRCVSet(void)
{
	if((cmdBuf[8] == SP) && (cmdBuf[13] == SP) && (cmdBuf[17] == CR) && (cmdBuf[18] == LF))
	{
		if((cmdBuf[9] == 0x20) && (cmdBuf[10] <= 0x99) && ((cmdBuf[11] >= 0x01) && (cmdBuf[11] <= 0x12)) &&
		((cmdBuf[12] >= 0x01) && (cmdBuf[12] <= 0x31)) && (cmdBuf[14] <= 0x23) && (cmdBuf[15] <= 0x59) && (cmdBuf[16] <= 0x59))
		{
			dateTime.year = cmdBuf[9] * 256 + cmdBuf[10];
			dateTime.month = cmdBuf[11];
			dateTime.day = cmdBuf[12];
			dateTime.hours = cmdBuf[14];
			dateTime.minutes = cmdBuf[15];
			dateTime.seconds = cmdBuf[16];
			rtcSet();
			rtcGet();
			return	OK;
		}
	}
	return	NG;
}

void	listTest(void)
{
	uint		cnt;
	uchar		AdrHi;
	uchar		AdrMid;
	uchar		AdrLow;

	sramCSLow();
	AdrHi = (uchar)(SRAM_INDEX_START_ADRESS >> 16);				// Adr Hi
	AdrMid = (uchar)(SRAM_INDEX_START_ADRESS >> 8);				// Adr Mid
	AdrLow = (uchar)(SRAM_INDEX_START_ADRESS & 0xff);			// Adr Low
	delay1us();

	UCA1TXBUF = 0x02;											// Write CMD
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = AdrHi;											// Adr Hi
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = AdrMid;											// Adr Mid
	while (!(UCA1IFG&UCTXIFG));									//
	UCA1TXBUF = AdrLow;											// Adr Low
	while (!(UCA1IFG&UCTXIFG));									//

	for( cnt=0; cnt < SYSTEM_INDEX_COUNT * SYSTEM_INDEX_BYTE; cnt++)
	{
//		UCA1TXBUF = testData;
		UCA1TXBUF = (uchar)cnt;
		while (!(UCA1IFG&UCTXIFG));								//
	}
	delay1us();
	sramCSHi();
//	SRAMClear(SRAM_INDEX_START_ADRESS);
//	sdAdrWrite();
	sdAdrRead();
//	listNo
}

#if 0
void	listTestDummy(void)
{
	uint	cnt;

	listNo = 5;
	listCount = 0;

	for(cnt = 0; cnt < listNo; cnt ++)
	{
		indexBuf[listCount++] = 0x20;
		indexBuf[listCount++] = 0x19;
		indexBuf[listCount++] = 0x06;
		indexBuf[listCount++] = 0x04;
		indexBuf[listCount++] = 0x08;
		indexBuf[listCount++] = 0x15;
		indexBuf[listCount++] = 0x17;
		indexBuf[listCount++] = 0x00;
	}
	for(cnt = 0; cnt < listNo; cnt ++)
	{
		listWork[0] = (cnt & 0xff00) >> 8;
		listWork[1] = cnt & 0x00ff;
		listWork[2] = SP;
		listWork[3] = indexBuf[0];
		listWork[4] = indexBuf[1];
		listWork[5] = indexBuf[2];
		listWork[6] = indexBuf[3];
		listWork[7] = SP;
		listWork[8] = indexBuf[4];
		listWork[9] = indexBuf[5];
		listWork[10] = indexBuf[6];
		listWork[11] = indexBuf[7];
		listWork[12] = CR;
		listWork[13] = LF;
		usbSerialAdrOut((uchar*) listWork, 13);
	}
}
#endif

void	dataCommand(void)
{
	if((cmdBuf[5] == SP) && (cmdBuf[8] == CR) && (cmdBuf[9] == LF))
	{
		dataNo = (uint)((cmdBuf[6] << 8) | cmdBuf[7]);
		usbDataSend();
	}
}

void	listCommand(void)
{
	uint	listcnt, cnt;

#if 0
	listcnt = 1;
#else
	for(listcnt = 0; listcnt < MAX_DATA_INDEX; listcnt ++)
	{
		if(indexDate[listcnt][0] == 0)
		{
			break;
		}
	}
#endif
	listWork[0] = ((listcnt) & 0xff00) >> 8;
	listWork[1] = (listcnt) & 0x00ff;
	listWork[2] = CR;
	listWork[3] = LF;
	usbSerialAdrOut((uchar*) listWork, 4);

#if 0
		cnt = 0;
		indexDate[cnt][0] = 0x20;
		indexDate[cnt][1] = (dateTime.year & 0x00ff);
		indexDate[cnt][2] = dateTime.month;
		indexDate[cnt][3] = dateTime.day;
		indexDate[cnt][4] = dateTime.hours;
		indexDate[cnt][5] = dateTime.minutes;
		indexDate[cnt][6] = dateTime.seconds;
		indexDate[cnt][7] = 0x00;

		listWork[0] = 0;
		listWork[1] = 1;
		listWork[2] = SP;
		listWork[3] = indexDate[cnt][0];
		listWork[4] = indexDate[cnt][1];
		listWork[5] = indexDate[cnt][2];
		listWork[6] = indexDate[cnt][3];
		listWork[7] = SP;
		listWork[8] = indexDate[cnt][4];
		listWork[9] = indexDate[cnt][5];
		listWork[10] = indexDate[cnt][6];
		listWork[11] = indexDate[cnt][7];
		listWork[12] = CR;
		listWork[13] = LF;
		usbSerialAdrOut((uchar*) listWork, 14);
#else
	for(cnt = 0; cnt < listcnt; cnt ++)
	{
		listWork[0] = ((cnt +1) & 0xff00) >> 8;
		listWork[1] = (cnt +1) & 0x00ff;
		listWork[2] = SP;
		listWork[3] = indexDate[cnt][0];
		listWork[4] = indexDate[cnt][1];
		listWork[5] = indexDate[cnt][2];
		listWork[6] = indexDate[cnt][3];
		listWork[7] = SP;
		listWork[8] = indexDate[cnt][4];
		listWork[9] = indexDate[cnt][5];
		listWork[10] = indexDate[cnt][6];
		listWork[11] = indexDate[cnt][7];
		listWork[12] = CR;
		listWork[13] = LF;
		usbSerialAdrOut((uchar*) listWork, 14);
	}
#endif
}

uchar	paraSetCommand(void)
{
	if((cmdBuf[8] == SP) && (cmdBuf[17] == SP) && (cmdBuf[19] == SP) && (cmdBuf[21] == SP) &&
	(cmdBuf[23] == SP) && (cmdBuf[25] == SP) && (cmdBuf[27] == SP) && (cmdBuf[29] == SP) &&
	(cmdBuf[31] == SP) && (cmdBuf[33] == SP) && (cmdBuf[35] == SP) && (cmdBuf[37] == SP) &&
	(cmdBuf[39] == SP) && (cmdBuf[41] == SP) && (cmdBuf[43] == SP) && (cmdBuf[47] == SP) &&
	(cmdBuf[51] == SP) && (cmdBuf[55] == SP) && (cmdBuf[59] == SP) && (cmdBuf[63] == SP) &&
	(cmdBuf[67] == SP) && (cmdBuf[71] == SP) && (cmdBuf[75] == SP) && (cmdBuf[79] == SP) &&
	(cmdBuf[83] == SP) && (cmdBuf[87] == SP) && (cmdBuf[91] == SP) && (cmdBuf[95] == SP) &&
	(cmdBuf[99] == SP) && (cmdBuf[103]) && (cmdBuf[107] == CR) && (cmdBuf[108] == LF))
	{
		memcpy(&svBuf[53],&cmdBuf[9],8);						// CH Enable
		memcpy(&svBuf[61],&cmdBuf[18],1);						// decimation
		memcpy(&svBuf[62],&cmdBuf[20],1);						// AD PGA Ch0
		memcpy(&svBuf[63],&cmdBuf[22],1);						// AD PGA Ch1
		memcpy(&svBuf[64],&cmdBuf[24],1);						// AD PGA Ch2
		memcpy(&svBuf[65],&cmdBuf[26],1);						// AD PGA Ch3
		memcpy(&svBuf[66],&cmdBuf[28],1);						// AD PGA Ch4
		memcpy(&svBuf[67],&cmdBuf[30],1);						// AD PGA Ch5
		memcpy(&svBuf[68],&cmdBuf[32],1);						// AD PGA Ch6
		memcpy(&svBuf[69],&cmdBuf[34],1);						// AD PGA Ch7
		memcpy(&svBuf[70],&cmdBuf[36],1);						// SD Mode(OverWrite/Stop)
		memcpy(&svBuf[71],&cmdBuf[38],1);						// Mode(Measurement Mode/Trigger Mode)
		memcpy(&svBuf[72],&cmdBuf[40],1);						// Trigger Rec Time
		memcpy(&svBuf[73],&cmdBuf[42],1);						// PreTrigger Rec Time
		memcpy(&svBuf[74],&cmdBuf[44],3);						// Trigger SV CH0
		memcpy(&svBuf[77],&cmdBuf[48],3);						// Trigger SV CH1
		memcpy(&svBuf[80],&cmdBuf[52],3);						// Trigger SV CH2
		memcpy(&svBuf[83],&cmdBuf[56],3);						// Trigger SV CH3
		memcpy(&svBuf[86],&cmdBuf[60],3);						// Trigger SV CH4
		memcpy(&svBuf[89],&cmdBuf[64],3);						// Trigger SV CH5
		memcpy(&svBuf[92],&cmdBuf[68],3);						// Trigger SV CH6
		memcpy(&svBuf[95],&cmdBuf[72],3);						// Trigger SV CH7
		memcpy(&svBuf[98],&cmdBuf[76],3);						// Trigger release SV CH0
		memcpy(&svBuf[101],&cmdBuf[80],3);						// Trigger release SV CH1
		memcpy(&svBuf[104],&cmdBuf[84],3);						// Trigger release SV CH2
		memcpy(&svBuf[107],&cmdBuf[88],3);						// Trigger release SV CH3
		memcpy(&svBuf[110],&cmdBuf[92],3);						// Trigger release SV CH4
		memcpy(&svBuf[113],&cmdBuf[96],3);						// Trigger release SV CH5
		memcpy(&svBuf[116],&cmdBuf[100],3);					// Trigger release SV CH6
		memcpy(&svBuf[119],&cmdBuf[104],3);					// Trigger release SV CH7
		return	OK;
	} else
	{
		return	NG;
	}
}

void	paraGetCommand(void)
{
	svBuf[0] = (uchar)((dateTime.year & 0x0ff00) >> 8);
	svBuf[1] = (uchar)dateTime.year & 0x0ff;
	svBuf[2] = (uchar)dateTime.month;
	svBuf[3] = (uchar)dateTime.day;
	svBuf[4] = (uchar)dateTime.hours;
	svBuf[5] = (uchar)dateTime.minutes;
	svBuf[6] = (uchar)dateTime.seconds;

	svBuf[35] = (uchar)(recPoint.lat_h & 0x000000ff);
	svBuf[36] = (uchar)((recPoint.lat_l & 0x0ff000000) >> 24);
	svBuf[37] = (uchar)((recPoint.lat_l & 0x000ff0000) >> 16);
	svBuf[38] = (uchar)((recPoint.lat_l & 0x00000ff00) >> 8);
	svBuf[39] = (uchar)(recPoint.lat_l & 0x0000000ff);
	svBuf[40] = (uchar)(recPoint.lng_h & 0x000000ff);
	svBuf[41] = (uchar)((recPoint.lng_l & 0x0ff000000) >> 24);
	svBuf[42] = (uchar)((recPoint.lng_l & 0x000ff0000) >> 16);
	svBuf[43] = (uchar)((recPoint.lng_l & 0x00000ff00) >> 8);
	svBuf[44] = (uchar)(recPoint.lng_l & 0x0000000ff);
	svBuf[45] = (uchar)((recPoint.alt & 0xff00) >> 8);
	svBuf[46] = (uchar)(recPoint.alt & 0xff);

	memcpy(&cmdBuf[0], &svBuf[0], 4);							// Date(YYYYMMDD)
	cmdBuf[4] = SP;
	memcpy(&cmdBuf[5], &svBuf[4], 3);							// Time(hhmmss)
	cmdBuf[8] = SP;
	memcpy(&cmdBuf[9], &svBuf[7], 20);							// Sirial No.
	cmdBuf[29] = SP;
	memcpy(&cmdBuf[30], &svBuf[27], 2);						// Ver No.(XX.XX)
	cmdBuf[32] = SP;
	memcpy(&cmdBuf[33], &svBuf[29], 6);						// SD Card Rec Capa/Max Rec Cap(XXX.XXX/XXX.XXX))
	cmdBuf[39] = SP;
	memcpy(&cmdBuf[40], &svBuf[35], 5);						// Rec Point(dddmmmmmmm)
	cmdBuf[45] = SP;
	memcpy(&cmdBuf[46], &svBuf[40], 5);						// (dddmmmmmmm)
	cmdBuf[51] = SP;
	memcpy(&cmdBuf[52], &svBuf[45], 2);						// (nnnn)
	cmdBuf[54] = SP;
	memcpy(&cmdBuf[55], &svBuf[47], 2);						// Temp(XXX.X)
	cmdBuf[57] = SP;
	memcpy(&cmdBuf[58], &svBuf[49], 2);						// Hum(XXX.X)
	cmdBuf[60] = SP;
	memcpy(&cmdBuf[61], &svBuf[51], 2);						// Status()
	cmdBuf[63] = SP;
	memcpy(&cmdBuf[64], &svBuf[53], 8);						// CH Enable
	cmdBuf[72] = SP;
	memcpy(&cmdBuf[73], &svBuf[61], 1);						// decimation
	cmdBuf[74] = SP;
	memcpy(&cmdBuf[75], &svBuf[62], 1);						// AD PGA Ch0
	cmdBuf[76] = SP;
	memcpy(&cmdBuf[77], &svBuf[63], 1);						// AD PGA Ch1
	cmdBuf[78] = SP;
	memcpy(&cmdBuf[79], &svBuf[64], 1);						// AD PGA Ch2
	cmdBuf[80] = SP;
	memcpy(&cmdBuf[81], &svBuf[65], 1);						// AD PGA Ch3
	cmdBuf[82] = SP;
	memcpy(&cmdBuf[83], &svBuf[66], 1);						// AD PGA Ch4
	cmdBuf[84] = SP;
	memcpy(&cmdBuf[85], &svBuf[67], 1);						// AD PGA Ch5
	cmdBuf[86] = SP;
	memcpy(&cmdBuf[87], &svBuf[68], 1);						// AD PGA Ch6
	cmdBuf[88] = SP;
	memcpy(&cmdBuf[89], &svBuf[69], 1);						// AD PGA Ch7
	cmdBuf[90] = SP;
	memcpy(&cmdBuf[91], &svBuf[70], 1);						// SD Mode(OverWrite/Stop)
	cmdBuf[92] = SP;
	memcpy(&cmdBuf[93], &svBuf[71], 1);						// Mode(Measurement Mode/Trigger Mode)
	cmdBuf[94] = SP;
	memcpy(&cmdBuf[95], &svBuf[72], 1);						// Trigger Rec Time
	cmdBuf[96] = SP;
	memcpy(&cmdBuf[97], &svBuf[73], 1);						// PreTrigger Rec Time
	cmdBuf[98] = SP;
	memcpy(&cmdBuf[99], &svBuf[74], 3);						// Trigger SV CH0
	cmdBuf[102] = SP;
	memcpy(&cmdBuf[103], &svBuf[77], 3);						// Trigger SV CH1
	cmdBuf[106] = SP;
	memcpy(&cmdBuf[107], &svBuf[80], 3);						// Trigger SV CH2
	cmdBuf[110] = SP;
	memcpy(&cmdBuf[111], &svBuf[83], 3);						// Trigger SV CH3
	cmdBuf[114] = SP;
	memcpy(&cmdBuf[115], &svBuf[86], 3);						// Trigger SV CH4
	cmdBuf[118] = SP;
	memcpy(&cmdBuf[119], &svBuf[89], 3);						// Trigger SV CH5
	cmdBuf[122] = SP;
	memcpy(&cmdBuf[123], &svBuf[92], 3);						// Trigger SV CH6
	cmdBuf[126] = SP;
	memcpy(&cmdBuf[127], &svBuf[95], 3);						// Trigger SV CH7
	cmdBuf[130] = SP;
	memcpy(&cmdBuf[131], &svBuf[98], 3);						// Trigger release SV CH0
	cmdBuf[134] = SP;
	memcpy(&cmdBuf[135], &svBuf[101], 3);						// Trigger release SV CH1
	cmdBuf[138] = SP;
	memcpy(&cmdBuf[139], &svBuf[104], 3);						// Trigger release SV CH2
	cmdBuf[142] = SP;
	memcpy(&cmdBuf[143], &svBuf[107], 3);						// Trigger release SV CH3
	cmdBuf[146] = SP;
	memcpy(&cmdBuf[147], &svBuf[110], 3);						// Trigger release SV CH4
	cmdBuf[150] = SP;
	memcpy(&cmdBuf[151], &svBuf[113], 3);						// Trigger release SV CH5
	cmdBuf[154] = SP;
	memcpy(&cmdBuf[155], &svBuf[116], 3);						// Trigger release SV CH6
	cmdBuf[158] = SP;
	memcpy(&cmdBuf[159], &svBuf[119], 3);						// Trigger release SV CH7
	cmdBuf[162] = SP;
	memcpy(&cmdBuf[163], &svBuf[122], 3);						// AD Offset CH0
	cmdBuf[166] = SP;
	memcpy(&cmdBuf[167], &svBuf[125], 3);						// AD Offset CH1
	cmdBuf[170] = SP;
	memcpy(&cmdBuf[171], &svBuf[128], 3);						// AD Offset CH2
	cmdBuf[174] = SP;
	memcpy(&cmdBuf[175], &svBuf[131], 3);						// AD Offset CH3
	cmdBuf[178] = SP;
	memcpy(&cmdBuf[179], &svBuf[134], 3);						// AD Offset CH4
	cmdBuf[182] = SP;
	memcpy(&cmdBuf[183], &svBuf[137], 3);						// AD Offset CH5
	cmdBuf[186] = SP;
	memcpy(&cmdBuf[187], &svBuf[140], 3);						// AD Offset CH6
	cmdBuf[190] = SP;
	memcpy(&cmdBuf[191], &svBuf[143], 3);						// AD Offset CH7
	cmdBuf[194] = SP;
	memcpy(&cmdBuf[195], &svBuf[146], 3);						// AD GAIN CH0
	cmdBuf[198] = SP;
	memcpy(&cmdBuf[199], &svBuf[149], 3);						// AD GAIN CH1
	cmdBuf[202] = SP;
	memcpy(&cmdBuf[203], &svBuf[152], 3);						// AD GAIN CH2
	cmdBuf[206] = SP;
	memcpy(&cmdBuf[207], &svBuf[155], 3);						// AD GAIN CH3
	cmdBuf[210] = SP;
	memcpy(&cmdBuf[211], &svBuf[158], 3);						// AD GAIN CH4
	cmdBuf[214] = SP;
	memcpy(&cmdBuf[215], &svBuf[161], 3);						// AD GAIN CH5
	cmdBuf[218] = SP;
	memcpy(&cmdBuf[219], &svBuf[164], 3);						// AD GAIN CH6
	cmdBuf[222] = SP;
	memcpy(&cmdBuf[223], &svBuf[167], 3);						// AD GAIN CH7
	cmdBuf[226] = CR;
	cmdBuf[227] = LF;
	usbSerialAdrOut((uchar*) cmdBuf, SV_COUNT);
}

void	resetCommand(void)
{
	OKStatusSend();
	error();
}

void	stopCommand(void)
{
	OKStatusSend();
	dataStart = OFF;
}

void	startCommand(void)
{
	OKStatusSend();
	dataStart = ON;
	sync1PPSFlag = OFF;
	syncStartFlag = ON;
	syncRsvFlag = ON;
	if((cmdBuf[6] == SP) && (cmdBuf[8] == CR) && (cmdBuf[9] == LF))
	{
		if(cmdBuf[7]  >= 0x90) cmdBuf[7] = cmdBuf[7] - 6 * 9;
		else if(cmdBuf[7]  >= 0x80) cmdBuf[7] = cmdBuf[7] - 6 * 8;
		else if(cmdBuf[7]  >= 0x70) cmdBuf[7] = cmdBuf[7] - 6 * 7;
		else if(cmdBuf[7]  >= 0x60) cmdBuf[7] = cmdBuf[7] - 6 * 6;
		else if(cmdBuf[7]  >= 0x50) cmdBuf[7] = cmdBuf[7] - 6 * 5;
		else if(cmdBuf[7]  >= 0x40) cmdBuf[7] = cmdBuf[7] - 6 * 4;
		else if(cmdBuf[7]  >= 0x30) cmdBuf[7] = cmdBuf[7] - 6 * 3;
		else if(cmdBuf[7]  >= 0x20) cmdBuf[7] = cmdBuf[7] - 6 * 2;
		else if(cmdBuf[7]  >= 0x10) cmdBuf[7] = cmdBuf[7] - 6 * 1;
		syncStartCounter = (uint)cmdBuf[7];
	} else
	{
		syncStartCounter = 0;
	}
	syncWaitFlag = ON;
	if(unitType == MASTER)
	{
//		Timer2Set(TIME_4S);
		Timer2Set(syncStartCounter * 4000);
	}
}

void	NGStatusSend(void)
{
	P1->OUT &= ~BIT4;

	UCB0TXBUF = (uchar)((NGStatus & 0xff00) >> 8);
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	UCB0TXBUF = (uchar)(NGStatus & 0x00ff);
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	crLFSend();
	delay1us();
	P1->OUT |= BIT4;
}

void	OKStatusSend(void)
{
	P1->OUT &= ~BIT4;

	UCB0TXBUF = (uchar)((OKStatus & 0xff00) >> 8);
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	UCB0TXBUF = (uchar)(OKStatus & 0x00ff);
	while (!(UCB0IFG&UCTXIFG));									// USCI_B0 TX buffer ready?
	crLFSend();
	delay1us();
	P1->OUT |= BIT4;
}

#if 0
void	NGSend(void)
{
	uint	cnt;

	P1->OUT &= ~BIT4;
	for(cnt = 0; cnt < sizeof(NGMSG); cnt++)
	{
		UCB0TXBUF = NGMSG[cnt];
		while (!(UCB0IFG&UCTXIFG));								// USCI_B0 TX buffer ready?
	}
	delay1us();
	P1->OUT |= BIT4;
}
#endif

#if 0
void	OKSend(void)
{
	uint	cnt;

	P1->OUT &= ~BIT4;
	for(cnt = 0; cnt < sizeof(OKMSG); cnt++)
	{
		UCB0TXBUF = OKMSG[cnt];
		while (!(UCB0IFG&UCTXIFG));								// USCI_B0 TX buffer ready?
	}
	delay1us();
	P1->OUT |= BIT4;
}
#endif

void	replySend(uchar rep)
{
	if(rep == OK)
	{
		OKStatusSend();
//		OKSend();
	} else
	{
		NGStatusSend();
//		NGSend();
	}
}

void	commandDataTransSet(void)
{
	uint	no = dataNo;

	sdStart = OFF;
	readSDExe = ON;
//	readSDStartAdr = (indexAdrBuf[no - 1]);				// Start sector +6
	readSDStartAdr = 0x00;				// Start sector +6
//	readSDLength = (indexAdrBuf[no]) - readSDStartAdr;
	readSDLength = 0xafc8;
	readSDAdr = 0;
	if(no == 0)
	{
		if(indexAdrBuf[MAX_DATA_INDEX - 1] == 0)			// Start = 1
		{
			readSDStartAdr = 0;
			readSDLength = (indexAdrBuf[no]) - readSDStartAdr;
		}
	}
	cmdStep = STEP7;
	cmdTransStep = STEP1;
}

void	commandDataTrans(void)
{
	uchar	rep;

	if(readSDAdr < readSDLength)
	{
		switch(cmdTransStep)
		{
			case STEP1:
				rep = sdBlockRead(readSDStartAdr + readSDAdr);
				if(rep != 0)
				{
					sdErrStatus = SD_READ_ERR;
				}
				cmdTransStep++;
				break;
			case STEP2:
				if(readSDDMAStatus == SDREADDMASTATUS00)
				{
					memcpy(usbWorkBuf, sdWorkReadBuf, SD_BLOCK_SIZE);
					usbDMADataSendSet();
					cmdTransStep++;
					cmdTransStep = STEP1;
					readSDAdr++;
				}
				break;
		}
	} else
	{
		readSDExe = OFF;
		sdStart = ON;
		cmdDataTrans = OFF;
		cmdTransStep = STEP0;
	}
}

void	commandCheck(void)
{
	uint	cnt;
	uchar	rep;

	P1->OUT &= ~BIT4;
	cmdDataTrans = OFF;
	for(cnt = 0; cnt < CMDLENGTH; cnt++)
	{
		dummyRX = (uchar)UCB0RXBUF;								// 01Byte
		UCB0TXBUF = dummyTX;
		while (!(UCB0IFG&UCRXIFG));								// USCI_B0 RX buffer ready?
		cmdBuf[cnt] = (uchar)UCB0RXBUF;
		if((cmdBuf[cnt] == LF) && (cmdBuf[cnt-1] == CR))
		{
			P1->OUT |= BIT4;
			if((memcmp(&cmdBuf[1], startCMD, sizeof(startCMD))) == 0)				// START
			{
				startCommand();
				return;
			} else if((memcmp(&cmdBuf[1], stopCMD, sizeof(stopCMD))) == 0)			// STOP
			{
				stopCommand();
				return;
			} else if((memcmp(&cmdBuf[1], resetCMD, sizeof(resetCMD))) == 0)		// RESET
			{
				resetCommand();
				return;
			} else if((memcmp(&cmdBuf[1], listCMD, sizeof(listCMD))) == 0)			// LIST
			{
				listCommand();
//				OKSend();
//				OKStatusSend();
				return;
			} else if((memcmp(&cmdBuf[1], dataCMD, sizeof(dataCMD))) == 0)			// DATA
			{
				OKStatusSend();
				cmdDataTrans = ON;
				dataCommand();
//				replySend(rep);
				return;
			} else if((memcmp(&cmdBuf[1], parasetCMD, sizeof(parasetCMD))) == 0)	// PARASET
			{
				rep = paraSetCommand();
				replySend(rep);
				return;
			} else if((memcmp(&cmdBuf[1], paragetCMD, sizeof(paragetCMD))) == 0)	// PARAGET
			{
				paraGetCommand();
				return;
			} else if((memcmp(&cmdBuf[1], offsetCMD, sizeof(offsetCMD))) == 0)		// OFFSET
			{
//				OKSend();
				OKStatusSend();
				return;
			} else if((memcmp(&cmdBuf[1], gainCMD, sizeof(gainCMD))) == 0)			// GAIN
			{
//				OKSend();
				OKStatusSend();
				return;
			} else if((memcmp(&cmdBuf[1], daytimeCMD, sizeof(daytimeCMD))) == 0)	// DAYTIME
			{
				rep = dayTimeRCVSet();
				replySend(rep);
				return;
			} else if((memcmp(&cmdBuf[1], selftestCMD, sizeof(selftestCMD))) == 0)	// SELFTEST
			{
//				OKSend();
				OKStatusSend();
				return;
			} else if((memcmp(&cmdBuf[1], statusCMD, sizeof(statusCMD))) == 0)		// STATUS
			{
				statusSend();
				return;
			}
		}
	}
	NGStatusSend();
//	NGSend();
	P1->OUT |= BIT4;
}

void	commandSelect(void)
{
	switch(cmdStep)
	{
		case  STEP1:
		case  STEP2:
			cmdStep += 1;
			break;
		case  STEP3:
			if(cmdBusy)
			{
				NGStatusSend();
//				NGSend();
				cmdStep = STEP0;
			} else
			{
//				OKSend();
				OKStatusSend();
				cmdStep += 1;
			}
			break;
		case  STEP4:
			if(!(P4->IN & BIT6))
			{
				cmdStep += 1;
			}
			break;
		case  STEP5:
			commandCheck();
			cmdStep = STEP6;
			break;
		case  STEP6:
			if(cmdDataTrans == ON)
			{
				commandDataTransSet();
			} else
			{
				cmdStep = STEP0;
			}
			break;
		case  STEP7:
			commandDataTrans();
			if(cmdDataTrans == OFF)
			{
				cmdStep = STEP0;
			}
	}
}


/****************************************
 *	memory.h							*
 ****************************************/

#include	"define.h"

typedef	unsigned	char	uchar;
typedef	unsigned	short	ushort;
typedef	unsigned	int		uint;
typedef	unsigned	long	ulong;

uint	unitType = UNITTYPE;

uint	sdBufAdr = 0;
uint	adBufAdr = 0;
uint	reSampleSetValue = NOWADRESAMP;
uint	reSampleLFSetValue = 1000;
uint	reSampleCount = 0;
uint	adpcmUse = USEADPCM;
uchar	adWorkBuf[AD_DATA_COUNT];
uchar	sdBuf[SD_BUFF_LENGTH];									//512*32*2	0x8000	32768
uchar	adBuf[AD_BUF_LENGTH];									//512*6		0xc00	3072
uchar	sdReadBuf[SD_BLOCK_SIZE];								//512		0x0200
uchar	sdWorkReadBuf[SD_BLOCK_SIZE];							//512		0x0200
uchar	sdWriteBuf[SD_BLOCK_SIZE];								//512		0x0200
//uchar	testReadBuf[SD_BLOCK_SIZE * SD_WRITE_SECTOR];			//512*6		0x0c00
uchar	usbWriteBuf[USB_WRITE_DEF_LENGTH];						//192		0x00c0
uchar	usbHeaderBuf[USB_HEADER_LENGTH];						//512		0x0200
uchar	usbWorkBuf[SD_BLOCK_SIZE];								//512		0x0200
uint	testReadAdr = 0;
uint	testPointer1 = 0;
uint	testPointer2 = 0;
uint	startAdr = 0;
uint	endAdr = 0;
uint	usbStartAdr = 0;
uint	usbEndAdr = 0;
uint	usbStartCount = 0;
uint	sramSaveStartAdr = 0;
uint	sramSaveEndAdr = 0;
uint	sramLoadStartAdr = 0;
uint	sramLoadEndAdr = 0;
ulong	sdMainCount = 0;
uint	usbWriteCountSV = NOWUSB_WRITE_LENGTH;
uchar*	writeBufAdr;
uchar	writeInstADRBuf[SRAM_INST_ADR_LENGTH];
uchar*	readBufAdr;
uchar	readInstADRBuf[SRAM_INST_ADR_LENGTH];
uchar	sramWorkReadBuf[SRAM_READ_LENGTH];
uchar	sramWriteBuf[SRAM_WRITE_BUF_LENGTH];
uchar	sramReadBuf[SRAM_LOAD_LENGTH];
uint	sramSel = 0;
ulong	sramAdr = 0;
ulong	sramReadAdr = 0;
uchar	sramAdrHi = 0;
uchar	sramAdrMid = 0;
uchar	sramAdrLow = 0;
uint	sramSaveStep = STEP0;
uint	sramLoadStep = STEP0;
uint	sramLoadSubStep = STEP0;
uchar	dummyData;
uint	dataPoint = 0x0;
uint	writeSRAMStatus = OFF;
uint	readSRAMStatus = OFF;
uint	writeSRAMDMAStatus = SRAMWRITEDMASTATUS00;
uint	readSRAMDMAStatus = SRAMREADDMASTATUS00;
uint	readSRAMDMAFlag = OFF;

uint	cmdWait = 0;
uchar	cmdBuf[CMDLENGTH];
uint	cmdStep = STEP0;
uint	cmdTransStep = STEP0;
uint	cmdReq = OFF;
uint	cmdBusy = OFF;
uint	cmdOK = OFF;
uint	cmdDataTrans = OFF;

uint	adStart = OFF;
uint	sdStart = OFF;
uint	sramStart = OFF;
uint	usbStart = OFF;
uint	timeGet = OFF;
uint	altGet = OFF;
uint	dataStart = OFF;
uint	dataSendStep = STEP0;
uint	dataPakeCnt = 0;
uint	sync1PPSFlag = 0;
uint	syncStartFlag = 0;
uint	syncRsvFlag = 0;
uint	syncStartCounter;
uint	syncMeasureStart = OFF;
uint	syncWaitFlag = 0;

uchar	rtcDefault = 0;

uchar	svBuf[SYSTEM_SV_COUNT];
uchar	recordIndexBuf[INDEX_POINTER];
uint	indexCount = 0;
uchar	indexDatePointer = 0;
uchar	indexDate[MAX_DATA_INDEX][8];
ulong	indexAdrBuf[MAX_DATA_INDEX];
uchar	indexStoreDate[8];
//uchar	testBuf[0x800];
//uchar	testBuf[0x1000];
//uchar	testData = 0;

uint	gpsCtr = 0;
uint	gpsPoint = 0;
uint	gpsLength = 0;
uchar	gpsTempBuffer[GPSBUFSIZE];
uchar	gpsBuffer[GPSBUFSIZE];

struct	dateTime
{
	uint	year;
	uint	month;
	uint	day;
	uint	dayOfmonth;
	uint	dayOfWeek;
	uint	hours;
	uint	minutes;
	uint	seconds;
} dateTime =
{
	0x2019, 0x01, 0x22, 0x01, 0x01, 0x08, 0x30, 0x00
};

struct	recPoint
{
	uint	lat_h;
	uint	lat_l;
	uint	lng_h;
	uint	lng_l;
	uint	alt;
} recPoint =
{
	0x00000000,0x00000000,0x0000
};

struct	trigger
{
	ulong	AVGCh0;
	ulong	AVGCh1;
	ulong	AVGCh2;
	ulong	AVGCh3;
	ulong	AVGCh4;
	ulong	AVGCh5;
	ulong	AVGCh6;
	ulong	AVGCh7;
	ulong	SVCh0;
	ulong	SVCh1;
	ulong	SVCh2;
	ulong	SVCh3;
	ulong	SVCh4;
	ulong	SVCh5;
	ulong	SVCh6;
	ulong	SVCh7;
}	trigger =
{
	0,0,0,0,0,0,0,0,TGS10,TGS10,TGS10,TGS10,TGS10,TGS10,TGS10,TGS10
};
uint	triggerSel = 0;
uint	triggerAdr = 0;
uchar	triggerBuf[3*8*10];

struct	Timer
{
	ulong	timer0;												// Command TimeOut
	uint	timer1;												// Reserve
	ulong	timer2;												// �ėp Timer
	uint	timer3;												// Reserve
	uint	timer4;												// Reserve
	uint	timer5;												// Reserve
	uint	timer6;												// Reserve
	uint	timer7;												// Reserve
	uint	timer8;												// Reserve
	uint	timer9;												// Reserve
	uint	timerA;												// Reserve
	uint	timerB;												// Reserve
	uint	timerC;												// Reserve
	uint	timerD;												// Reserve
	uint	timerE;												// Reserve
	uint	timerF;												// Reserve
} Timer =
{
//	0 1 2 3 4 5 6 7 8 9 A B C D E F
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

uint	time01MS = EVENT_TIME_01MS;
uint	time1250US = EVENT_TIME_1250US;
uint	time02MS = EVENT_TIME_02MS;
uint	time05MS = EVENT_TIME_05MS;
uint	time10MS = EVENT_TIME_10MS;
uint	time16MS = EVENT_TIME_16MS;
uint	time20MS = EVENT_TIME_20MS;
uint	time40MS = EVENT_TIME_40MS;
uint	time100MS = EVENT_TIME_100MS;
uint	time200MS = EVENT_TIME_200MS;
uint	time1S = EVENT_TIME_1S;
uint	time60S = EVENT_TIME_60S;

uint	eventTime01MS = CLR;
uint	eventTime1250US = CLR;
uint	eventTime02MS = CLR;
uint	eventTime05MS = CLR;
uint	eventTime10MS = CLR;
uint	eventTime16MS = CLR;
uint	eventTime20MS = CLR;
uint	eventTime40MS = CLR;
uint	eventTime100MS = CLR;
uint	eventTime200MS = CLR;
uint	eventTime1S = CLR;
uint	eventTime60S = CLR;

struct	readAdRegStatus
{
	uint				r00;									//Ch0
	uint				r01;									//Ch1
	uint				r02;									//Ch2
	uint				r03;									//Ch3
	uint				r04;									//Ch4
	uint				r05;									//Ch5
	uint				r06;									//Ch6
	uint				r07;									//Ch7
	uint				r08;									//Disable ADC Channel
	uint				r09;									//
	uint				r0a;									//
	uint				r0b;									//
	uint				r0c;									//
	uint				r0d;									//
	uint				r0e;									//
	uint				r0f;									//
	uint				r10;									//
	uint				r11;									//
	uint				r12;									//
	uint				r13;									//
	uint				r14;									//Data Output Format
	uint				r15;									//
	uint				r16;									//
	uint				r17;									//
	uint				r18;									//
	uint				r19;									//
	uint				r1a;									//
	uint				r1b;									//
	uint				r1c;									//
	uint				r1d;									//
	uint				r1e;									//
	uint				r1f;									//
	uint				r20;									//
	uint				r21;									//
	uint				r22;									//
	uint				r23;									//
	uint				r24;									//
	uint				r25;									//
	uint				r26;									//
	uint				r27;									//
	uint				r28;									//
	uint				r29;									//
	uint				r2a;									//
	uint				r2b;									//
	uint				r2c;									//
	uint				r2d;									//
	uint				r2e;									//
	uint				r2f;									//
	uint				r30;									//
	uint				r31;									//
	uint				r32;									//
	uint				r33;									//
	uint				r34;									//
	uint				r35;									//
	uint				r36;									//
	uint				r37;									//
	uint				r38;									//
	uint				r39;									//
	uint				r3a;									//
	uint				r3b;									//
	uint				r3c;									//
	uint				r3d;									//
	uint				r3e;									//
	uint				r3f;									//
	uint				r40;									//
	uint				r41;									//
	uint				r42;									//
	uint				r43;									//
	uint				r44;									//
	uint				r45;									//
	uint				r46;									//
	uint				r47;									//
	uint				r48;									//
	uint				r49;									//
	uint				r4a;									//
	uint				r4b;									//
	uint				r4c;									//Ch0 Status
	uint				r4d;									//Ch1 Status
	uint				r4e;									//Ch2 Status
	uint				r4f;									//Ch3 Status
	uint				r50;									//Ch4 Status
	uint				r51;									//Ch5 Status
	uint				r52;									//Ch6 Status
	uint				r53;									//Ch7 Status
	uint				r54;
	uint				r55;
	uint				r56;
	uint				r57;
	uint				r58;
	uint				r59;
	uint				r5a;
	uint				r5b;
	uint				r5c;
	uint				r5d;
	uint				r5e;
	uint				r5f;
	uint				r60;
	uint				r61;
	uint				r62;
	uint				r63;
}	readAdRegStatus;

struct	adChHeader
{
	uchar				Ch0;
	uchar				Ch1;
	uchar				Ch2;
	uchar				Ch3;
	uchar				Ch4;
	uchar				Ch5;
	uchar				Ch6;
	uchar				Ch7;

	uchar				Ch0ERR;
	uchar				Ch1ERR;
	uchar				Ch2ERR;
	uchar				Ch3ERR;
	uchar				Ch4ERR;
	uchar				Ch5ERR;
	uchar				Ch6ERR;
	uchar				Ch7ERR;
}	adChHeader;

struct	SDCard
{
	uchar	Type;												// SD:0/SDHC:1
	uint	DetCnt;
	uchar	DetSts;
//	uint	WPCnt;
//	uchar	WPSts;
	uchar	Err_Flag;											// error:0/OK:1
	uchar	Speed;												// LowSpeed:0/HighSpeed:1
} SDCard =
{
	SDV1,0,INIT,OK,SD_LOW_SPEED
//	SDV1,0,ON,0,ON,OK,SD_LOW_SPEED
};
ulong	sdFileCount = 0;
uchar	sdReg_buffer[16];
ulong	sd_sectors;
ulong	sdMaxSectors;
void*	sdPointer;
uint	sdStep = STEP0;
uint	sdSubStep = STEP0;
uint	sdStatusStep = STEP0;
uint	testSDStatus = SD_TST_STS_0;
uint	writeSDStatus = SDWRITESTATUS00;
uint	writeSDStatusChk = 0;
uint	writeSDDMAStatus = SDWRITEDMASTATUS00;
uint	writeSDBusyChk = OFF;
uint	writeSDBusyCnt = 1000;
uint	readSDStatus = SDREADSTATUS00;
uint	readSDExe = OFF;
uint	readSDCount = 0;
uint	readSDDMAStatus = SDREADDMASTATUS00;
uint	readSDBusyChk = OFF;
uint	readSDBusyCnt = 1000;
uint	readSDStartAdr = 0;
uint	readSDLength = 0;
uint	readSDAdr = 0;

uint	writeSDExe = OFF;
uint	writeUSBExe = OFF;
uint	writeUSBEnd = OFF;

ulong	sdCount = 0;
ulong	testCount = 0;
ulong	testSDCount = 0;										//test1204
ulong	testADCount = 0;										//test1204
ulong	sramSaveCount = 0;
ulong	sramLoadCount = 0;
ulong	sramLoadOK = OFF;
ulong	testData = 0;
uchar	testDataH = 0;
uchar	testDataL = 0;
uint	testFlag = 0;											//test0612
ulong	testUsbData = 0;
uint	testSRAMRead = 0;
uint	testSRAMWrite = 0;

uint	keyCount = 0;
uchar	dataEnable = OFF;

uint	txCount = 0;
uint	txData = 0;

uchar	dumData = 0;
uchar	dummyTX = 0x00;
uchar	dummyRX = 0xff;

//uchar	sdCardResetCMD[6] =		{0x40,0x00,0x00,0x00,0x00,0x95};			//CMD0 = 0x00 | 0x40 = 0x40
//uchar	sdCardInitCMD[6] =		{0x41,0x00,0x00,0x00,0x00,0xf9};			//CMD1 = 0x01 | 0x40 = 0x41
//uchar	sdCardSelectCMD[6] =	{0x48,0x00,0x00,0x01,0xaa,0x87};			//CMD8 = 0x08 | 0x40 = 0x48
//uchar	sdCardOSRCMD[6] =		{0x7a,0x00,0x00,0x00,0x00,0x00};			//CMD58 = 0x3a | 0x40 = 0x7a
//uchar	sdCardCIDCMD[6] =		{0x4a,0x00,0x00,0x00,0x00,0x00};			//CMD10 = 0x0a | 0x40 = 0x4a

//uchar	sdOSRBuf[4];
//uchar	sdCIDBuf[16];

uchar	DUMMY_DMA = BIT1;										/* P8.1 is Dummy Chip Select */
uchar	ADC_CS = BIT2;											/* P6.2 is AD Chip Select */

uchar	sdErrStatus = NO_ERR;
uchar	adErrStatus = NO_ERR;
uchar	usbErrStatus = NO_ERR;
uchar	sramErrStatus = NO_ERR;
uchar	adErrRetry = 0;

uint	enco_adpcm_val = 0;
uint	deco_adpcm_val = 0;
int		bits_per_code = 4;
signed int	PrevSample;											// Predicted sample
int		 PrevStepSize;											// Index into step size table

uchar	respSave = 0;
//uint	statusCode = 0;
uchar	statusCode = 0;
uint	clockCount = 0;
uint	clockCountOld = 0;
uint	capCount = 0;
uint	capCountOld = 0;
uint	calCount = 0;

uchar	listNo = 0;
uint	listCount = 0;
uchar	listWork[14];
uint	dataNo = 0;
uint	polarityFlag = 0;

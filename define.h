/****************************************
 *	define.h							*
 ****************************************/

#define		UNITTYPE			MASTER
//#define		UNITTYPE			SLAVE
#define		MASTER				1
#define		SLAVE				0
#define		DEBUG				1

#define		AD_CLK_PULS			0x04							//
#define		SD_CLK_PULS_INIT	0xf0							//
#define		SD_CLK_PULS			0x04							//
#define		SRAM_CLK_PULS		0x04							//
#define		USB_CLK_PULS		0x04							//
#define		SURFACE				1
//#define		OUTCLK				0xff						//
#define		AD_MSG_LENGTH		4
#define		SD_WRITE_LENGTH		SD_BLOCK_SIZE*SD_WRITE_SECTOR	//512*64
#define		SD_BUFF_LENGTH		SD_BLOCK_SIZE*SD_WRITE_SECTOR	//512*64=32768
#define		USART_MSG_LENGTH	960
#define		USB_HEADER_LENGTH	512
#define		NOWUSB_WRITE_LENGTH	192/(NOWADRESAMP+1)
#define		USB_WRITE_LENGTH00	192/(ADRESAMP00+1)
#define		USB_WRITE_LENGTH02	192/(ADRESAMP02+1)
#define		USB_WRITE_LENGTH04	192/(ADRESAMP04+1)
#define		USB_WRITE_LENGTH08	192/(ADRESAMP08+1)
#define		USB_WRITE_LENGTH16	192/(ADRESAMP16+1)
#define		USB_WRITE_DEF_LENGTH	192
#define		COPY_LENGTH			128

#define		AD_READ_COUNT		4*(8-1)							//4Byte*8Ch
#define		AD_DATA_COUNT		3*8								//3Byte*8Ch
#define		AD_BUF_LENGTH		AD_DATA_COUNT*128				//3Byte*8Ch*128=3072

#define		SRAM_WRITE_LENGTH	512
#define		SRAM_READ_LENGTH	64
#define		SRAM_LENGTH			512*6
#define		SRAM_INST_ADR_LENGTH	4
#define		SRAM_WRITE_BUF_LENGTH	SRAM_WRITE_LENGTH*3			//512*3=1536
//#define		SRAM_READ_BUF_LENGTH	SRAM_READ_LENGTH*256	//64*256=16384
#define		SRAM_READ_BUF_LENGTH	SRAM_WRITE_LENGTH*32			//512*3=1536

#define		ADRX_TASKS			65
#define		ADTX_TASKS			2
#define		SDRXTASKS			3
#define		SDTXTASKS			2
#define		USBTXTASKS			2
//#define		USBTXTASKS			9
#define		SRAMTXTASKS			5
//#define		SRAMTXTASKS			6
#define		SRAMRXTASKS00			5
#define		SRAMRXTASKS01			129
//#define		SRAMRXTASKS			9
#define		COPYTASKS			2
#define		RX1_TASKS			129
#define		RX2_TASKS			129
#define		RX3_TASKS			129
#define		RX4_TASKS			129
#define		RX5_TASKS			129
#define		RX6_TASKS			129
#define		RX7_TASKS			129
#define		RX8_TASKS			129
#define		TX1_TASKS			2
#define		TX2_TASKS			2
#define		TX3_TASKS			2
#define		TX4_TASKS			2
#define		TX5_TASKS			2
#define		TX6_TASKS			2
#define		TX7_TASKS			2
#define		TX8_TASKS			2

#define		SDWRITESTATUS00				0
#define		SDREADSTATUS00				0
#define		SDREADSTATUS01				1
#define		SDREADSTATUS02				2
#define		SDREADSTATUS03				3
#define		SDREADSTATUS04				4
#define		SDREADSTATUS05				5
#define		SDREADDMASTATUS00			0
#define		SDREADDMASTATUS01			1
#define		SDREADDMASTATUS02			2
#define		SDREADDMASTATUS03			3
#define		SDREADDMASTATUS04			4
#define		SDREADDMASTATUS05			5
#define		SDREADDMASTATUS06			6
#define		SDREADDMASTATUS07			7
#define		SDREADDMASTATUS08			8
#define		SDWRITEDMASTATUS00			0
#define		SDWRITEDMASTATUS01			1
#define		SDWRITEDMASTATUS02			2
#define		SDWRITEDMASTATUS03			3
#define		SDWRITEDMASTATUS04			4
#define		SDWRITEDMASTATUS05			5
#define		SDWRITEDMASTATUS06			6
#define		SDWRITEDMASTATUS07			7
#define		SDWRITEDMASTATUS08			8
#define		SRAMREADDMASTATUS00			0
#define		SRAMREADDMASTATUS01			1
#define		SRAMREADDMASTATUS02			2
#define		SRAMREADDMASTATUS03			3
#define		SRAMREADDMASTATUS04			4
#define		SRAMREADDMASTATUS05			5
#define		SRAMREADDMASTATUS06			6
#define		SRAMREADDMASTATUS07			7
#define		SRAMREADDMASTATUS08			8
#define		SRAMREADDMASTATUS09			9
#define		SRAMREADDMASTATUS10			10
#define		SRAMREADDMASTATUS11			11
#define		SRAMREADDMASTATUS12			12
#define		SRAMREADDMASTATUS13			13
#define		SRAMREADDMASTATUS14			14
#define		SRAMREADDMASTATUS15			15
#define		SRAMREADDMASTATUS16			16
#define		SRAMREADDMASTATUS17			17
#define		SRAMREADDMASTATUS18			18
#define		SRAMREADDMASTATUS19			19
#define		SRAMREADDMASTATUS20			20
#define		SRAMREADDMASTATUS21			21
#define		SRAMREADDMASTATUS22			22
#define		SRAMREADDMASTATUS23			23
#define		SRAMREADDMASTATUS24			24
#define		SRAMREADDMASTATUS25			25
#define		SRAMREADDMASTATUS26			26
#define		SRAMREADDMASTATUS27			27
#define		SRAMREADDMASTATUS28			28
#define		SRAMREADDMASTATUS29			29
#define		SRAMREADDMASTATUS30			30
#define		SRAMREADDMASTATUS31			31
#define		SRAMREADDMASTATUS32			32
#define		SRAMREADDMASTATUS33			33
#define		SRAMREADDMASTATUS34			34
#define		SRAMREADDMASTATUS35			35
#define		SRAMREADDMASTATUS36			36
#define		SRAMREADDMASTATUS37			37
#define		SRAMREADDMASTATUS65			65
#define		SRAMREADDMASTATUS257		257
#define		SRAMWRITEDMASTATUS00		0
#define		SRAMWRITEDMASTATUS01		1
#define		SRAMWRITEDMASTATUS02		2
#define		SRAMWRITEDMASTATUS03		3
#define		SRAMWRITEDMASTATUS04		4
#define		SRAMWRITEDMASTATUS05		5
#define		SRAMWRITEDMASTATUS06		6
#define		SRAMWRITEDMASTATUS07		7
#define		SRAMWRITEDMASTATUS08		8

#define		MAP_DMA_assignChannel			DMA_assignChannel
#define		MAP_DMA_assignInterrupt			DMA_assignInterrupt
#define		MAP_DMA_clearInterruptFlag		DMA_clearInterruptFlag
#define		MAP_DMA_disableChannel			DMA_disableChannel
#define		MAP_DMA_disableInterrupt		DMA_disableInterrupt
#define		MAP_DMA_enableChannel			DMA_enableChannel
#define		MAP_DMA_enableInterrupt			DMA_enableInterrupt
#define		MAP_DMA_enableModule			DMA_enableModule
#define		MAP_DMA_setChannelScatterGather	DMA_setChannelScatterGather
#define		MAP_DMA_setControlBase			DMA_setControlBase
#define		MAP_Interrupt_enableInterrupt	Interrupt_enableInterrupt
#define		MAP_Interrupt_disableInterrupt	Interrupt_disableInterrupt
#define		MAP_SPI_clearInterruptFlag		SPI_clearInterruptFlag

#define		USBDMACH	1
#define		USBDMACHSPI	DMA_CH1_EUSCIB0RX0
#define		SRAMDMACH	3
#define		SRAMDMACHSPI	DMA_CH3_EUSCIA1RX
#define		ADDMACH		5
#define		ADDMACHSPI	DMA_CH5_EUSCIB2RX0
#define		SDDMACH		7
#define		SDDMACHSPI	DMA_CH7_EUSCIB3RX0

#define		SYSTEM_SV_AREA					0x00038000
#define		SYSTEM_SV_COUNT					512
#define		SYSTEM_INDEX_AREA1				0x00039000
#define		SYSTEM_INDEX_AREA2				0x0003A000
#define		SYSTEM_INDEX_AREA3				0x0003B000
#define		SYSTEM_DAYTIME_AREA1			0x0003C000
#define		SYSTEM_DAYTIME_AREA2			0x0003D000
#define		SYSTEM_DAYTIME_AREA3			0x0003E000
#define		SYSTEM_DAYTIME_AREA4			0x0003F000
#define		SYSTEM_INDEX_COUNT				2048
#define		SYSTEM_INDEX_BYTE				5
#define		SYSTEM_DAYTIME_BYTE				8
#define		SYSTEM_INDEX_POINTER			128

#define		SV_COUNT						228
#define		PARA_GET_LENGTH					228

#define		SRAM_ALL_COUNT					0x000200
#define		SRAM_PRETRIGGER_START_ADRESS	0x000000
#define		SRAM_PRETRIGGER_STOP_ADRESS		0x0752ff
#define		SRAM_INDEX_CHECK_ADRESS			0x079770
#define		SRAM_INDEX_POINTER_ADRESS		0x079780
#define		SRAM_INDEX_START_ADRESS			0x079800
#define		SRAM_INDEX_STOP_ADRESS			0x07bfff
#define		SRAM_DAYTIME_START_ADRESS		0x07c000
#define		SRAM_DAYTIME_STOP_ADRESS		0x07ffff
#define		SRAM_CHECK_COUNT				10
#define		SRAM_INDEX_POINTER				128
#define		SRAM_MAX_ADRESS					0x080000

#define		INDEX_POINTER					128

#define		MAX_DATA_INDEX					10


#define		EVENT_TIME_01MS					1*4-1
#define		EVENT_TIME_1250US				1
#define		EVENT_TIME_02MS					2*4-1
#define		EVENT_TIME_05MS					5*4-1
#define		EVENT_TIME_10MS					10*4-1
#define		EVENT_TIME_16MS					16*4-1
#define		EVENT_TIME_20MS					20*4-1
#define		EVENT_TIME_40MS					40*4-1
#define		EVENT_TIME_100MS				100*4-1
#define		EVENT_TIME_200MS				200*4-1
#define		EVENT_TIME_1S					1000*4-1
#define		EVENT_TIME_60S					60000*4-1

#define		TIME_1S							1000*4				// 1sec
#define		TIME_2S							2000*4				// 2sec
#define		TIME_4S							4000*4				// 4sec
#define		TIME_5S							5000*4				// 5sec
#define		TIME_8S							8000*4				// 8sec
#define		TIME_100S						100000*4			// 100sec
#define		TIME_MUGEN						0xffffffff			// TimeUp–³

#define		CMDLENGTH			256
#define		GPSBUFSIZE			150


#define		ON					1
#define		OFF					0
#define		INIT				4
#define		MID2				2
#define		MID3				3
#define		OK					0
#define		NG					0xff
#define		TRUE				1
#define		FALSE				0
#define		SET					1
#define		CLR					0
#define		OUTCLK				0x00

#define		CR					0x0d
#define		LF					0x0a
#define		SP					0x20
#define		ESC					0x1b

#define		STEP0				0
#define		STEP1				1
#define		STEP2				2
#define		STEP3				3
#define		STEP4				4
#define		STEP5				5
#define		STEP6				6
#define		STEP7				7
#define		STEP8				8
#define		STEP9				9

#define		CH0					0
#define		CH1					1
#define		CH2					2
#define		CH3					3
#define		CH4					4
#define		CH5					5
#define		CH6					6
#define		CH7					7

#define		SDV1				0								// Check’†
#define		SDV2				1								// Check’†
#define		ERROR				2								//
#define		SDHC				3								// Check’†
#define		SDDET				4								// SD Detect

#define		SD_INIT_SUCCESS							0x00
#define		SD_INIT_FAIL							0x01
#define		SD_SetBL_SUCCESS						0x02
#define		SD_SetBL_ERROR							0x03
#define		SD_ReadREG_SUCCESS						0x04
#define		SD_RESPONSE_ERROR						0x05
#define		SD_BLOCK_SET_ERROR						0x06
#define		SD_CRC_ERROR							0x07
#define		SD_WRITE_ERROR							0x08
#define		SD_OTHER_ERROR							0x09
#define		SD_WRITE_SUCCESS						0x0A
#define		SD_READ_SUCCESS							0x0B
#define		SD_READ_DATA_TOKEN_ERROR				0x0C
#define		SD_READ_RESPONSE_ERROR					0x0D
#define		SD_READ_BLOCK_SET_ERROR					0x0E
#define		SD_Read_Write_Verification_Fail			0x0F
#define		SD_Read_Write_Verification_Success		0x10
#define		sdMemory_less_than_64MB					0x20
#define		sdMemory_64MB							0x21
#define		sdMemory_128MB							0x22
#define		sdMemory_256MB							0x23
#define		sdMemory_512MB							0x24
#define		sdMemory_1GB							0x25
#define		sdMemory_2GB							0x26
#define		sdMemory_4GB							0x27
#define		sdMemory_8GB							0x28
#define		sdMemory_16GB							0x29
#define		sdMemory_32GB							0x2A
#define		sdMemory_64GB							0x2B
#define		sdMemory_128GB							0x2C
#define		sdMemory_more_than_128GB				0x2D
#define		sdMemory_Calculation_Error				0x2E

#define		SD_START_DATA_BLOCK_TOKEN 				0xFE		// Data token start byte, Start Single Block Read

#define		SD_NSS_Low		P2->OUT &= ~BIT4;					// CS Off
#define		SD_NSS_High		P2->OUT |= BIT4;					// CS On

#define		SRAM1_CS_Low		P8->OUT &= ~BIT2;				// CS Off
#define		SRAM1_CS_High		P8->OUT |= BIT2;				// CS On
#define		SRAM2_CS_Low		P8->OUT &= ~BIT5;				// CS Off
#define		SRAM2_CS_High		P8->OUT |= BIT5;				// CS On
#define		SRAM3_CS_Low		P6->OUT &= ~BIT0;				// CS Off
#define		SRAM3_CS_High		P6->OUT |= BIT0;				// CS On
#define		SRAM4_CS_Low		P4->OUT &= ~BIT0;				// CS Off
#define		SRAM4_CS_High		P4->OUT |= BIT0;				// CS On

#define		ILLEGAL_CMD								0x04

#define		SD_INIT_OK			0x00
#define		SD_INIT_NG			0x01
#define		SD_INIT_RTY			30
#define		SD_LOW_SPEED		0								//400kbps
#define		SD_HIGH_SPEED		1								//12Mbps

//#define		SD_FILE_SIZE		1875							// 24(AD)*8000(sps)*30(sec)=5760000/3072
#define		SD_FILE_SIZE		7500							// 24(AD)*8000(sps)*120(sec)=23040000/3072
//#define		SD_FILE_SIZE		112500							// 24(AD)*8000(sps)*1800(sec)=345600000/3072
#define		SD_BLOCK_SIZE		512								// 512 bytes
#define		SD_READ_SIZE		512/8							// 64 bytes
#define		SD_MAX_SECTOR		6								// sector
#define		SD_WRITE_SECTOR		64							// sector
#define		SD_READ_SECTOR		6								// sector
#define		SD_COMP_SECTOR		6								// sector
#define		SD_TST_STS_0		0								//Test Status 0
#define		SD_TST_STS_1		1								//Test Status 1
#define		SD_TST_STS_2		2								//Test Status 2
#define		SD_TST_STS_3		3								//Test Status 3
#define		SD_TST_STS_4		4								//Test Status 4
#define		SD_TST_STS_5		5								//Test Status 5
#define		SD_WR_STS_0			0								//Write Status 0
#define		SD_WR_STS_1			1								//Write Status 1
#define		SD_WR_STS_2			2								//Write Status 2
#define		SD_WR_STS_3			3								//Write Status 3
#define		SD_WR_STS_4			4								//Write Status 4
#define		SD_WR_STS_5			0								//Write Status 5
#define		SD_WR_STS_6			1								//Write Status 6
#define		SD_WR_STS_7			2								//Write Status 7
#define		SD_WR_STS_8			3								//Write Status 8
#define		SD_RD_STS_0			0								//Read Status 0
#define		SD_RD_STS_1			1								//Read Status 1
#define		SD_RD_STS_2			2								//Read Status 2
#define		SD_RD_STS_3			3								//Read Status 3
#define		SD_RD_STS_4			4								//Read Status 4

#define		NO_ERR				0
#define		DEF_ERR				0xff

#define		SD_INIT_ERR			1
#define		SD_READ_ERR			2
#define		SD_WRIT_ERR			3
#define		SD_COMP_ERR			4
#define		SD_OTHER_ERR		5

#define		AD_HEAD_ERR			10

#define		USB_ERR				20

#define		AD_RETRY_ERR_COUNT	5


#if 0
#define		ADHEAD0				0x88
#define		ADHEAD1				0x98
#define		ADHEAD2				0xA8
#define		ADHEAD3				0xB8
#define		ADHEAD4				0xC8
#define		ADHEAD5				0xD8
#define		ADHEAD6				0xE8
#define		ADHEAD7				0xF8
#else
#define		ADHEAD0				0x80
#define		ADHEAD1				0x90
#define		ADHEAD2				0xA0
#define		ADHEAD3				0xB0
#define		ADHEAD4				0xC0
#define		ADHEAD5				0xD0
#define		ADHEAD6				0xE0
#define		ADHEAD7				0xF0
#endif

//#define		NOWADRESAMP			ADRESAMP00
#define		NOWADRESAMP			ADRESAMP00
#define		ADRESAMP00			0
#define		ADRESAMP02			2-1
#define		ADRESAMP04			4-1
#define		ADRESAMP08			8-1
#define		ADRESAMP16			16-1

#define		USEADPCM			OFF

#define		TGS05				0x199999
#define		TGS10				0x333333
#define		TGS15				0x4CCCCC
#define		TGS20				0x666666
#define		TGS25				0x7FFFFF

#define		DEFYEAR				2019
#define		DEFMONTH			04
#define		DEFDAY				01
#define		DEFDAYOFWEEK		3
#define		DEFHOUR				00
#define		DEFMINUTE			00
#define		DEFSECOND			0

#define		DELAY01MS			0x7b0
#define		FLASHCHECK1			0xA5
#define		FLASHCHECK2			0x5A

#define		SDWCNT				6
#define		TESTBLOCK			512
#define		TESTWCNT			3
#define		TESTRCNT			32
#define		TESTWTIME			100
#define		SRAM_SAVE_LENGTH	TESTBLOCK*TESTWCNT
#define		SRAM_LOAD_LENGTH	SD_BLOCK_SIZE * 3	//512*32
//#define		SRAM_LOAD_LENGTH	SD_BLOCK_SIZE * TESTWCNT	//512*3
#define		TESTWSECTORT		64

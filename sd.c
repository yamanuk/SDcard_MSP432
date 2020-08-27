/****************************************
 *	sd.c								*
 ****************************************/

#include	"msp.h"
#include	"define.h"
#include	"extern.h"
#include	"sd.h"

extern	void	readSDBlock(void);
extern	void	writeSDBlock(void);
//extern	void	sdWriteMain(void);
extern	void	SDSPISel(void);
extern	void	delay1us(void);
extern	void	delay5us(void);
extern	void	delay10us(void);
extern	void	delay30us(void);
extern	void	delay100us(void);
extern	void	delay1ms(void);
extern	void	delay100ms(void);
extern	void	ledD1Off(void);
extern	void	ledD1On(void);
extern	void	ledD1OnOff(void);
extern	void	led2Off(void);
extern	void	led2On(void);
extern	void	led2OnOff(void);
extern	void	testPort1Off(void);
extern	void	testPort1On(void);
extern	void	testPort1OnOff(void);

void	sdSpeedLowSet(void);
void	sdSpeedHighSet(void);
void	sdCheck(void);
uchar	sdSetBlockLength(void);
uchar	sdReadRegister(const uchar cmd_register, const uchar length_in_bytes);
uchar	sdhcCapacity(void);
uchar	sdCapacity(void);
uchar	sdSendByte(uchar spiDataByte);
void	sendCrc(void);
void	writeOut(void);
void	writeBlock(uchar *buf);
void	readBlock(uchar *buf);
int		calc_sd_crc7(char *buf);
void	sdSendCmd(const uchar cmd, ulong data, const char crc);
uchar	sdGetResponseCMD58(void);
uchar	sdGetResponseCMD8(void);
uchar	sdGetResponseCMD(void);
uchar	sdGetResponse(void);
uchar	sdCheckBusy(void);
uchar	sdWRBusyChk(void);
uchar	sdWRStsRead(void);
//void	sdReadMain(void);
uchar	sdBlockMultiRead(long blocknum);
uchar	sdBlockRead(long blocknum);
//uchar	sdBlockRead(long blocknum, uchar *buf);
//void	sdWriteMain(void);
uchar	sdBlockMultiWrite(long blocknum, uint count,  uchar *buf);
uchar	sdBlockMultiWrite1(long blocknum, uint count);
uchar	sdBlockMultiWrite2(uint count, uchar *buf);
uchar	sdBlockMultiWrite3(void);
uchar	sdBlockWrite(long blocknum, uchar *buf);
uchar	sdInit(void);
uchar	sdInitSub(void);
void	initSd(void);

void	sdSpeedLowSet(void)
{
	SDCard.Speed = SD_LOW_SPEED;								// LowSpeed:0/HighSpeed:1
	SDSPISel();													// SPISelect
}

void	sdSpeedHighSet(void)
{
	SDCard.Speed = SD_HIGH_SPEED;								// LowSpeed:0/HighSpeed:1
	SDSPISel();													// SPISelect
}

void	sdCheck(void)
{
//SD Detect
	if(P2->IN & BIT5)
	{
		if((SDCard.DetSts == OFF) || (SDCard.DetSts == MID3))
		{
			SDCard.DetSts = MID2;
			SDCard.DetCnt = 0;
		} else if(SDCard.DetSts == MID2)
		{
			SDCard.DetCnt++;
			if(SDCard.DetCnt == 3)
			{
				SDCard.DetSts = ON;
			}
		}
	} else
	{
		if((SDCard.DetSts == ON) || (SDCard.DetSts == MID2))
		{
			SDCard.DetSts = MID3;
			SDCard.DetCnt = 0;
		} else if(SDCard.DetSts == MID3)
		{
			SDCard.DetCnt++;
			if(SDCard.DetCnt == 3)
			{
				SDCard.DetSts = OFF;
			}
		}
	}
//SD WP
#if 0
	if(P2->IN & BIT6)
	{
		if((SDCard.WPSts == OFF) || (SDCard.WPSts == MID3))
		{
			SDCard.WPSts = MID2;
			SDCard.WPCnt = 0;
		} else if(SDCard.WPSts == MID2)
		{
			SDCard.WPCnt++;
			if(SDCard.WPCnt == 3)
			{
				SDCard.WPSts = ON;
			}
		}
	} else
	{
		if((SDCard.WPSts == ON) || (SDCard.WPSts == MID2))
		{
			SDCard.WPSts = MID3;
			SDCard.WPCnt = 0;
		} else if(SDCard.WPSts == MID3)
		{
			SDCard.WPCnt++;
			if(SDCard.WPCnt == 3)
			{
				SDCard.WPSts = OFF;
			}
		}
	}
#endif
}

uchar	sdSetBlockLength(void)
{
	// Max. read data block length and Max. write data block length according to CSD Register Fields
	// is 512 bytes, i.e. one sector (512-byte sector)
	// Set the block length to read
	sdSendCmd(16, SD_BLOCK_SIZE, 0xFF);	// Send CMD16
	// get response from SD - make sure that its 0x00 (R1 ok response format)
	if(sdGetResponse()!=0x00){
		// Send 8 Clock pulses of delay.
		sdSendByte(0xFF);
		return SD_SetBL_ERROR;
	}
	// Send 8 Clock pulses of delay.
	sdSendByte(0xFF);
	return SD_SetBL_SUCCESS;
}

uchar	sdReadRegister(const uchar cmd_register, const uchar length_in_bytes)
{
	uint	uc = 0;
	uchar	rvalue = SD_RESPONSE_ERROR;
	// In SPI Mode only CID, CSD and OCR register are accessible!!! [p.g. 68]
	// Reading the contents of the OCR,CID or CSD registers in SPI mode is a simple read-block transaction.
	// The card will respond with a standard response token followed by a data block of 16 bytes,
	// suffixed with a 16-bit CRC. [p.g. 72]

	SD_NSS_Low;	// Set CS low, i.e. select card
	// Send CMD register read
	// CRC not used: 0xFF as last byte
	// Read and write commands have data transfers associated with them. Data is being
	// transmitted or received via data tokens. All data bytes are transmitted MSB first. [p.g. 80]
	sdSendCmd(cmd_register, 0x00000000, 0xFF);
	// wait for response
	// in the R1 format (0x00 is no errors)
	if (sdGetResponse() == 0x00)
	{
		if(cmd_register != 58)
		{	// If CID or CSD are read registers:
			// After R1 response, First read byte is: Start Block Token(At Single Block Read it is: 0xFE). [p.g. 80]
			if(sdGetResponse() == 0xFE)
			{
				// The next bytes are: 2-(N+1): User data. [p.g. 80]
				for (uc = 0; uc < length_in_bytes ; uc++)
				{
					sdReg_buffer[15-uc] = sdSendByte(0xFF);
				}
				// And Last two bytes are: 16-bit CRC. [p.g. 80]
				// get CRC bytes (not really needed by us, but required by SD)
				sendCrc();
				rvalue = SD_ReadREG_SUCCESS;
			} else 	rvalue = SD_RESPONSE_ERROR;
		// Error: Card hasn't sent Start Block Token.
		} else {	// If read register is OCR, i.e. CMD58
			// register read operation is: <39..31>R1, <31..0>OCR -> R3 format
			for (uc = 0; uc < length_in_bytes ; uc++)
			{
				sdReg_buffer[15-uc] = sdSendByte(0xFF);
			}
		// And Last two bytes are: 16-bit CRC. [p.g. 80]
		// get CRC bytes (not really needed by us, but required by SD)
		sendCrc();
		rvalue = SD_ReadREG_SUCCESS;
		}
	}
	else rvalue = SD_RESPONSE_ERROR;
	// Error: In card R1 response, or card hasn't sent R1 response.
	// Send 8 Clock pulses of delay.
	 sdSendByte(0xFF);
	// Send 8 Clock pulses of delay.
	sdSendByte(0xFF);
	SD_NSS_High;
	return rvalue;
}				// sd_read_register

uchar	sdhcCapacity(void){
	uint	C_SIZE;
	uchar	READ_BL_LEN;
	uint	BLOCK_LEN;
	ulong	BLOCKNR;
	ulong	Memory_capacity;
	
	SD_NSS_Low;	// Set CS low, i.e. select card
	//extract READ_BL_LEN from sdReg_buffer
	READ_BL_LEN = sdReg_buffer[10] & 0x0F;
	if(READ_BL_LEN > 11)	return sdMemory_Calculation_Error;
	//extract C_SIZE from sdReg_buffer
	C_SIZE = (sdReg_buffer[8] & 0xfC) << 6;
	C_SIZE = (C_SIZE | sdReg_buffer[7]) << 8;
	C_SIZE = (C_SIZE | sdReg_buffer[6]);						// Length is 22 bits, so max value is: 0x3FFFFF = 4194303
//	if(C_SIZE > 4095)	return sdMemory_Calculation_Error;

	// Insert equations from SanDisk datasheet: ProdManRS-SDv1.3 page 31
	BLOCK_LEN = 1 << READ_BL_LEN;								// Max value: 2^15 = 32767(0x7fff)
	BLOCKNR = (C_SIZE+1);										// Max value: (4194303+1) = 4194304(0x400000)
	Memory_capacity = BLOCKNR * ((ulong)BLOCK_LEN);				// Max value: 4194304*32767 = 137438953471(0x1fffffffff)
	sd_sectors = Memory_capacity;
	sdMaxSectors = Memory_capacity;
	SD_NSS_High;	// Set CS high, i.e. deselect card

	if(Memory_capacity <= 63000) 			return sdMemory_less_than_64MB;
	else if(Memory_capacity <= 65000)			return sdMemory_64MB;
	else if(Memory_capacity <= 129000)			return sdMemory_128MB;
	else if(Memory_capacity <= 257000)			return sdMemory_256MB;
	else if(Memory_capacity <= 513000)			return sdMemory_512MB;
	else if(Memory_capacity <= 1100000)			return sdMemory_1GB;
	else if(Memory_capacity <= 2100000)			return sdMemory_2GB;
	else if(Memory_capacity <= 4100000)			return sdMemory_4GB;
	else if(Memory_capacity <= 8100000)			return sdMemory_8GB;
	else if(Memory_capacity <= 17000000)		return sdMemory_16GB;
	else if(Memory_capacity <= 33000000)		return sdMemory_32GB;
	else if(Memory_capacity <= 65000000)		return sdMemory_64GB;
	else if(Memory_capacity <= 130000000)		return sdMemory_128GB;
	else 										return sdMemory_more_than_128GB;
}

uchar	sdCapacity(void){
	uint	C_SIZE;
	uchar	C_SIZE_MULT;
	uchar	READ_BL_LEN;
	uint	MULT;
	uint	BLOCK_LEN;
	ulong	BLOCKNR;
	ulong	Memory_capacity;
	uchar	Temp;
	
	SD_NSS_Low;	// Set CS low, i.e. select card
	//extract READ_BL_LEN from sdReg_buffer
	READ_BL_LEN = sdReg_buffer[10] & 0x0F;
	if(READ_BL_LEN > 11)	return sdMemory_Calculation_Error;
	//extract C_SIZE_MULT from sdReg_buffer
	C_SIZE_MULT = sdReg_buffer[6] &0x03;
	C_SIZE_MULT = C_SIZE_MULT << 1;
	if(sdReg_buffer[5] & 0x80)	C_SIZE_MULT++;
	//extract C_SIZE from sdReg_buffer
	C_SIZE = (sdReg_buffer[9] & 0x03) << 8;
	C_SIZE = (C_SIZE | sdReg_buffer[8]) << 2;
	Temp = (sdReg_buffer[7] & 0xC0) >> 6;
	C_SIZE = C_SIZE | Temp;					// Length is 12 bits, so max value is: 0xFFF = 4095
	if(C_SIZE > 4095)	return sdMemory_Calculation_Error;

	// Insert equations from SanDisk datasheet: ProdManRS-SDv1.3 page 31
	Temp = C_SIZE_MULT + 2;
	MULT = 1 << Temp;	// This is the true equation! There is an error in datasheet! MULT Max value: 512
	BLOCK_LEN = 1 << READ_BL_LEN;								// Max value: 2^11 = 2048(0x800)
	BLOCKNR = (C_SIZE+1) * ((ulong)MULT);						// Max value: (4095+1)*512 = 2097152(0x200000)
	Memory_capacity = BLOCKNR * ((ulong)BLOCK_LEN);				// Max value: 2097152*2048 = 4294967296(0x100000000)
	sd_sectors = Memory_capacity;
	sdMaxSectors = BLOCKNR;
	SD_NSS_High;	// Set CS high, i.e. deselect card

	if(Memory_capacity <= 63000000) 			return sdMemory_less_than_64MB;
	else if(Memory_capacity <= 65000000)		return sdMemory_64MB;
	else if(Memory_capacity <= 129000000)		return sdMemory_128MB;
	else if(Memory_capacity <= 257000000)		return sdMemory_256MB;
	else if(Memory_capacity <= 513000000)		return sdMemory_512MB;
	else if(Memory_capacity <= 1100000000)		return sdMemory_1GB;
	else if(Memory_capacity <= 2100000000)		return sdMemory_2GB;
	else if(Memory_capacity <= 4100000000)		return sdMemory_4GB;
	else if(Memory_capacity <= 8100000000)		return sdMemory_8GB;
	else if(Memory_capacity <= 17000000000)		return sdMemory_16GB;
	else if(Memory_capacity <= 33000000000)		return sdMemory_32GB;
	else if(Memory_capacity <= 65000000000)		return sdMemory_64GB;
	else if(Memory_capacity <= 130000000000)	return sdMemory_128GB;
	else 										return sdMemory_more_than_128GB;
}

uchar	sdSendByte(uchar spiDataByte)
{
	UCB3TXBUF = (uint)spiDataByte;
	while (!(UCB3IFG&UCRXIFG));
	spiDataByte = (uchar)UCB3RXBUF;
	return spiDataByte;
}

void	sendCrc(void)
{
	sdSendByte(0xFF);
	sdSendByte(0xFF);
}

void	readBlock(uchar *buf)
{
	uint	i;

	// clock the actual data transfer and receive the bytes;
	for (i = 0; i < SD_BLOCK_SIZE; i++)
	{
		UCB3TXBUF = (uint)0xff;
		while (!(UCB3IFG&UCRXIFG));
		buf[i] = (uchar)UCB3RXBUF;
	}
	// get CRC bytes (not really needed by us, but required by MMC)
}

void	writeOut(void)
{
	uint	i;

	for (i = 0; i < SRAM_READ_LENGTH; i++)
	{
		UCB3TXBUF = (uint)sramWorkReadBuf[i];
		while (!(UCB3IFG&UCRXIFG));
		dumData = (uchar)UCB3RXBUF;
	}
}

void	writeBlock(uchar *buf)
{
	uint	i;

	// clock the actual data transfer and receive the bytes;
	for (i = 0; i < SD_BLOCK_SIZE; i++)
	{
		UCB3TXBUF = (uint)buf[i];
		while (!(UCB3IFG&UCRXIFG));
		dumData = (uchar)UCB3RXBUF;
	}
	// get CRC bytes (not really needed by us, but required by MMC)
}

int		calc_sd_crc7(char *buf)
{
	int		crc, crc_prev;
	int		i,j;

	crc = buf[0];
	for(i = 1; i < 6; i++)
	{
		for(j = 7; j >= 0; j--)
		{
			crc <<= 1;
			crc_prev = crc;
			if (i < 5) crc |= (buf[i] >> j) & 1;
			if (crc & 0x80)
			{
				 crc ^= 0x89;
			 }
		}
	}
	return crc_prev | 1;
}

void	sdSendCmd(const uchar cmd, ulong data, const char crc)
{
	char	frame[6];
	uchar	temp;
	uint	i;

	frame[0]=(char)(0x40 | cmd);
	for(i = 1; i <= 4; i++)
	{
		temp=(char)(data>>(8*(4-i)));
		frame[i]=(temp);
	}
	frame[5] = calc_sd_crc7(&frame[0]);
	for(i=0;i<6;i++) sdSendByte(frame[i]);						// Send command.
}

uchar	sdGetResponseCMD58(void)
{
	uint	i = 0;
	uint	retry = 0;
	uchar	reply = 0;
	uchar	response = ERROR;
	uchar	responsebuf[5];

	while(retry <= 64)
	{
		reply = sdSendByte(0xFF);
		if(reply == 0x01)
		{
			responsebuf[0] = reply;
			for(i = 1; i < 5; i++)
			{
				responsebuf[i] = sdSendByte(0xFF);
			}
			if((responsebuf[1] & ~0x40) != 0x00) break;
			if(responsebuf[2] != 0xFF) break;
			if(responsebuf[3] != 0x80) break;
			if(responsebuf[4] != 0x00) break;
			response = OK;
			break;
		}
		retry++;
	}
	sdSendByte(0xFF);
	if((responsebuf[1] & 0x40) != 0x00)
	{
		SDCard.Type = SDHC;
	}
	return response;
}

uchar	sdGetResponseCMD8(void)
{
	uint	i = 0;
	uint	retry = 0;
	uchar	reply = 0;
	uchar	response = ERROR;
	uchar	k[7];
	
	for(i=0;i<=6;i++)
	    k[i]=0x00;

	while(retry <= 64)
	{
		/*
		reply = sdSendByte(0xFF);
		if(reply == 0x05)										// SD = ILLEGAL_CMD+BUSY?
		{
			response = SDDET;
			break;
		} else if(reply == 0x01)
		{
			responsebuf[0] = reply;
			for(i = 1; i < 5; i++)
			{
				responsebuf[i] = sdSendByte(0xFF);
			}
			if(responsebuf[1] != 0x00) break;
			if(responsebuf[2] != 0x00) break;
			if(responsebuf[3] != 0x01) break;
			if(responsebuf[4] != 0xAA) break;
			response = SDHC;
			break;
		} else
		{
			responsebuf[0] = reply;
		}
		*/
		k[0] = k[1];
		k[1] = k[2];
		k[2] = k[3];
		k[3] = k[4];
		k[4] = k[5];
		k[5] = k[6];
		k[6] = sdSendByte(0xFF);
		//if response is R7 
		//39~32bit : R1 response / 11~8bit : 設定電圧(CMD8時) / 7~0bit: 
		//if(k[0]==0x00 & k[3]==0x01 & k[4]==0xAA){
		if(k[2] != 0x00){ //1bitずれているけどこれじゃないと、if文が認識してくれない＞＞明らかにおかしいです
			response = SDHC;
	//		break;
		}
		retry++;
	}
//	sdSendByte(0xFF);
	return response;
}

uchar	sdGetResponseCMD(void)
{
	//Response comes 1-8bytes after command
	//the first bit will be a 0
	//followed by an error code
	//data will be 0xFF until response
	uint	retry = 0;
	uchar	response;

	while(retry <= 64)
	{
		response = sdSendByte(0xFF);
		if(response == 0x00)break;
		if(response == 0x01)break;
		if(response == 0xFE)break;
		retry++;
	}
	sdSendByte(0xFF);
	return response;
}

uchar	sdGetResponse(void)
{
	//Response comes 1-8bytes after command
	//the first bit will be a 0
	//followed by an error code
	//data will be 0xFF until response
	uint	retry = 0;
	uchar	response;

	while(retry <= 5000)
	{
		response = sdSendByte(0xFF);
		if(response == 0x00)break;
		if(response == 0x01)break;
		if(response == 0xFE)break;
		retry++;
	}
	return response;
}

uchar sdCheckBusy(void)
{
	uint	i = 0;
	uchar	response;
	uchar	rvalue;
	// After a data block is received, the MultiMediaCard/RS-MultiMediaCard will respond with a data-response
	// token, and if the data block is received with no errors, it will be programmed. A continuous stream of
	// busy tokens will be sent to the host (effectively holding the dataOut line low) for the duration  of
	// time the card is busy, programming. [p.g. 71]

	// Get Card Data Response Token:
	while(i <= 64)
	{
		// Waiting Data Response from card
		// Data Response Format: xxx0<status>1
		// Status = 010 優ata accepted.
		// Status = 101 優ata rejected due to a CRC error.
		// Status = 110 優ata rejected due to a write error.
		response = sdSendByte(0xFF);
		response &= 0x1F;
		switch(response)
		{
			case 0x05: rvalue = SD_WRITE_SUCCESS;break;
			case 0x0B: return	SD_CRC_ERROR;
			case 0x0D: return	SD_WRITE_ERROR;
			default:
				rvalue = SD_OTHER_ERROR;
				break;
		}
	if(rvalue==SD_WRITE_SUCCESS)break;
	i++;
	}
	i = 0;
	// Wait until card stop sending busy token stream:
	// because a continuous stream of busy tokens will be sent to the host,
	// (effectively holding the dataOut line low) for the duration of time the card is busy, programming.
	do
	{
		response = sdSendByte(0xFF);
		i++;
		if(i == 65000)
		{
			// Error due to card doesn't stop sending of busy token stream.
			rvalue = SD_OTHER_ERROR;
			break;
		}
	}while(response==0);
	return rvalue;
}

uchar	sdWRBusyChk(void)
{
	uchar	rvalue;

	rvalue = sdSendByte(0xFF);
	return rvalue;
}

uchar	sdWRStsRead(void)
{
	uint	i = 0;
	uchar	response;
	uchar	rvalue;

	// Get Card Data Response Token:
	while(i <= 65535)
	{
		// Waiting Data Response from card
		// Data Response Format: xxx0<status>1
		// Status = 010 優ata accepted.
		// Status = 101 優ata rejected due to a CRC error.
		// Status = 110 優ata rejected due to a write error.
		response = sdSendByte(0xFF);
		response &= 0x1F;
		switch(response)
		{
			case 0x05:
				rvalue = SD_WRITE_SUCCESS;break;
			case 0x0B:
				return	SD_CRC_ERROR;
			case 0x0D:
				return	SD_WRITE_ERROR;
			default:
				rvalue = SD_OTHER_ERROR;
				break;
		}
		if(rvalue==SD_WRITE_SUCCESS)break;
		i++;
	}
	return rvalue;
}

//******************************************************************************
// ブロックリード
//******************************************************************************
#if 0
void	sdReadMain(void)
{
#if 0
	uchar	rep;

	rep = sdBlockMultiRead(sdCount);
	if(rep != 0)
	{
		sdErrStatus = SD_READ_ERR;
	}
//	while(readSDExe != OFF);
#else
	uchar	rep;
	uint	cnt;

	switch(readSDStatus)
	{
		case SDREADSTATUS00:
			readSDExe = ON;
			readSDCount = 0;
			readSDStatus++;
			break;
		case SDREADSTATUS01:
			rep = sdBlockRead(sdCount + readSDCount);
			if(rep != 0)
			{
				sdErrStatus = SD_READ_ERR;
			}
			readSDStatus++;
			break;
		case SDREADSTATUS02:
			if(readSDDMAStatus == SDREADDMASTATUS00)
			{
				for(cnt = 0; cnt < SD_BLOCK_SIZE; cnt++)
				{
					sdWriteBuf[SD_BLOCK_SIZE *  readSDCount+ cnt] = sdReadBuf[cnt];
				}
				readSDStatus++;
			}
			break;
		case SDREADSTATUS03:
			readSDCount++;
			if(readSDCount < SD_READ_SECTOR)
			{
				readSDStatus = SDREADSTATUS01;
			} else
			{
				readSDStatus = SDREADSTATUS00;
			}
			break;
	}

#endif
}
#endif

uchar	sdBlockMultiRead(long blocknum)
{
	uchar	rep;

	SD_NSS_Low;	// Set CS low, i.e. select card
	// send read command SD_READ_SINGLE_BLOCK=CMD17
	if(SDCard.Type == SDHC)
	{
		sdSendCmd (18,blocknum, 0xFF);
	} else
	{
		sdSendCmd (18,blocknum * SD_BLOCK_SIZE, 0xFF);
	}
	// Check if the MMC acknowledged the read block command
	// it will do this by sending an affirmative response R1 to CMD17
	// in the R1 format (0x00 is no errors)
	rep = sdGetResponse();
	if (rep == 0x00)
	{
		// now look for the Single Block Read (0xFE) token to signify the start of the data
		rep = sdGetResponse();
		if (rep == SD_START_DATA_BLOCK_TOKEN)
		{
//			readBlock((uchar*) buf + SD_BLOCK_SIZE * pointer);
			readSDBlock();
		} else {	// Timeout Exit
			SD_NSS_High;										// Set CS high, i.e. deselect card
			return 0xFF;
		}
	} else {
		SD_NSS_High;											// Set CS high, i.e. deselect card
		return 0xFF;
	}
	return 0;
}


#if 0
uchar	sdBlockRead(long blocknum, uchar *buf)
{
	uchar	rep;

	SD_NSS_Low;	// Set CS low, i.e. select card
	// send read command SD_READ_SINGLE_BLOCK=CMD17
	if(SDCard.Type == SDHC)
	{
		sdSendCmd (17,blocknum, 0xFF);
	} else
	{
		sdSendCmd (17,blocknum * SD_BLOCK_SIZE, 0xFF);
	}
	// Check if the MMC acknowledged the read block command
	// it will do this by sending an affirmative response R1 to CMD17
	// in the R1 format (0x00 is no errors)
	rep = sdGetResponse();
	if (rep == 0x00)
	{
		// now look for the Single Block Read (0xFE) token to signify the start of the data
		rep = sdGetResponse();
		if (rep == SD_START_DATA_BLOCK_TOKEN)
		{
			readBlock((uchar*) buf);
			sendCrc();
		} else {	// Timeout Exit
			SD_NSS_High;										// Set CS high, i.e. deselect card
			return 0xFF;
		}
	} else {
		SD_NSS_High;											// Set CS high, i.e. deselect card
		return 0xFF;
	}
	sdSendByte(0xFF);
	SD_NSS_High;												// Set CS high, i.e. deselect card
	return 0;
#else
uchar	sdBlockRead(long blocknum)
{
	uchar	rep;

	SD_NSS_Low;	// Set CS low, i.e. select card
	// send read command SD_READ_SINGLE_BLOCK=CMD17
	if(SDCard.Type == SDHC)
	{
		sdSendCmd (17,blocknum, 0xFF);
	} else
	{
		sdSendCmd (17,blocknum * SD_BLOCK_SIZE, 0xFF);
	}
	// Check if the MMC acknowledged the read block command
	// it will do this by sending an affirmative response R1 to CMD17
	// in the R1 format (0x00 is no errors)
	rep = sdGetResponse();
	if (rep == 0x00)
	{
		// now look for the Single Block Read (0xFE) token to signify the start of the data
		rep = sdGetResponse();
		if (rep == SD_START_DATA_BLOCK_TOKEN)
		{
			readBlock((uchar*) sdReadBuf);
			sdSendByte(0xFF);
			sdSendByte(0xFF);
//			readSDBlock();
		} else {	// Timeout Exit
			sdSendByte(0xFF);
			SD_NSS_High;										// Set CS high, i.e. deselect card
			sdSendByte(0xFF);
			return 0xFF;
		}
	} else {
		sdSendByte(0xFF);
		SD_NSS_High;											// Set CS high, i.e. deselect card
		sdSendByte(0xFF);
		return 0xFF;
	}
	sdSendByte(0xFF);
	SD_NSS_High;												// Set CS high, i.e. deselect card
	sdSendByte(0xFF);
	return 0;
}
#endif

//******************************************************************************
// ブロックライト
//******************************************************************************
void	sdBlockMultiWriteSub(void)
{
	uchar	rep;

	sdSendByte(0xFC);	// Send Start Block Token
//	writeBlock(buf);
	writeSDBlock();
	while(writeSDExe == ON);
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
		writeSDExe = OFF;
}

uchar	sdBlockMultiWrite(long blocknum, uint count, uchar *buf)
{
	uint	CMD1_counter = 0x00;
	uint	point;
	uchar	response = 0x01;
	uchar	rep;
	SD_NSS_Low;	// Set CS low, i.e. select card

	while(response != 0x00)										// 
	{	
		sdSendCmd(55,0x00,0x65);								// 
		rep = sdGetResponseCMD();
		if (rep == ERROR)
		{
			SD_NSS_High;	// Set CS low, i.e. select card
			return 0xFF;
		}
		sdSendCmd(23, count, 0xFF);
		response = sdGetResponseCMD();
		CMD1_counter++;
		if(CMD1_counter == 10000)
		{
			SD_NSS_High;	// Set CS low, i.e. select card
			return 0xFF;
		}
	}
	// send write command
	if(SDCard.Type == SDHC)
	{
		sdSendCmd(25, blocknum, 0xFF);
	} else
	{
		sdSendCmd(25, blocknum * SD_BLOCK_SIZE, 0xFF);
	}
	// check if the MMC acknowledged the write block command
	// it will do this by sending an affirmative response R1
	// in the R1 format (0x00 is no errors)
	rep = sdGetResponse();
	if (rep == 0x00)
	{
		for(point = 0; point < count; point++)
		{
			memcpy(sdWriteBuf, buf, SD_BLOCK_SIZE);
			sdBlockMultiWriteSub();
			buf += SD_BLOCK_SIZE;
			if(buf >= (uchar*) (sdBuf + SD_BUFF_LENGTH))
			{
				buf -= SD_BUFF_LENGTH;
			}
		}
		sdSendByte(0xFD);
		rep = sdGetResponse();
		if(rep != 0x00)
		{
			sdErrStatus = SD_WRIT_ERR;
		}
		SD_NSS_High;												// Set CS high, i.e. deselect card
		sdSendByte(0xFF);
	} else {
		// the MMC never acknowledge the write command
		SD_NSS_High;	// Set CS low, i.e. select card
		sdSendByte(0xFF);
		return 0xFF;
	}
	SD_NSS_High;	// Set CS low, i.e. select card
	SD_NSS_Low;												// Set CS low, i.e. select card
	while(rep == 0)
	{
		rep = sdWRBusyChk();
	}
	SD_NSS_High;											// Set CS high, i.e. deselect card
	sdSendByte(0xFF);
	return 0;
}

uchar	sdBlockMultiWrite1(long blocknum, uint count)
{
	uint	CMD1_counter = 0x00;
//	uint	point;
	uchar	response = 0x01;
	uchar	rep;
	SD_NSS_Low;	// Set CS low, i.e. select card

	while(response != 0x00)										// 
	{	
		sdSendCmd(55,0x00,0x65);								// 
		rep = sdGetResponseCMD();
		if (rep == ERROR)
		{
			SD_NSS_High;	// Set CS low, i.e. select card
			return 0xFF;
		}
		sdSendCmd(23, count, 0xFF);
		response = sdGetResponseCMD();
		CMD1_counter++;
		if(CMD1_counter == 10000)
		{
			SD_NSS_High;	// Set CS low, i.e. select card
			return 0xFF;
		}
	}
	// send write command
	if(SDCard.Type == SDHC)
	{
		sdSendCmd(25, blocknum, 0xFF);
	} else
	{
		sdSendCmd(25, blocknum * SD_BLOCK_SIZE, 0xFF);
	}
	// check if the MMC acknowledged the write block command
	// it will do this by sending an affirmative response R1
	// in the R1 format (0x00 is no errors)
	rep = sdGetResponse();
	if (rep == 0x00)
	{
		SD_NSS_Low;	// Set CS low, i.e. select card
		return 0;
	} else {
		// the MMC never acknowledge the write command
		SD_NSS_High;	// Set CS low, i.e. select card
		sdSendByte(0xFF);
		return 0xFF;
	}
}

uchar	sdBlockMultiWrite2(uint count, uchar *buf)
{
	uint	point;

ledD1On();
	for(point = 0; point < count; point++)
	{
		memcpy(sdWriteBuf, buf, SD_BLOCK_SIZE);
		sdBlockMultiWriteSub();
		buf += SD_BLOCK_SIZE;
		if(buf >= (uchar*) (sdBuf + SD_BUFF_LENGTH))
		{
			buf -= SD_BUFF_LENGTH;
		}
	}
ledD1Off();
	return 0;
}

uchar	sdBlockMultiWrite3(void)
{
	uchar	rep;

	sdSendByte(0xFD);
	rep = sdGetResponse();
	if(rep != 0x00)
	{
		sdErrStatus = SD_WRIT_ERR;
	}
	SD_NSS_High;											// Set CS high, i.e. deselect card
	sdSendByte(0xFF);
	SD_NSS_High;	// Set CS low, i.e. select card

	SD_NSS_Low;	// Set CS low, i.e. select card

	while(rep == 0)
	{
		rep = sdWRBusyChk();
	}
	SD_NSS_High;											// Set CS high, i.e. deselect card
	sdSendByte(0xFF);
	return 0;
}

uchar	sdBlockWrite(long blocknum, uchar *buf)
{
	uchar	rep;

	// send write command
	if(SDCard.Type == SDHC)
	{
		sdSendCmd (24,blocknum, 0xFF);
	} else
	{
		sdSendCmd (24,blocknum * SD_BLOCK_SIZE, 0xFF);
	}
	// check if the MMC acknowledged the write block command
	// it will do this by sending an affirmative response R1
	// in the R1 format (0x00 is no errors)
	rep = sdGetResponse();
	if (rep == 0x00)
	{
		// send the data token to signify the start of the data
		sdSendByte(0xFE);	// Send Start Block Token
		// clock the actual data transfer and transmit the bytes
		writeBlock((uchar*)buf);
//		writeSDBlock();
		sendCrc();
		rep = sdCheckBusy();
		if(rep != SD_WRITE_SUCCESS)
		{
			return 0xFF;
		}
	} else {
		// the MMC never acknowledge the write command
		return 0xFF;
	}
	SD_NSS_High;												// Set CS high, i.e. deselect card
	return 0;
}

uchar	sdInitSub(void)
{
	uint	i;
	uchar	sd_size;
	ulong	OCR_value = 0x00000000;
	uchar	reply = 0;

	// 4. Read OCR register!!!
	reply = sdReadRegister(58,4);								// OCR Register read
	if(reply == SD_ReadREG_SUCCESS)
	{
		for(i = 0; i <= 3; i++)
		{
			OCR_value = OCR_value | sdReg_buffer[15-i];
			if(i==3)	break;
			OCR_value = OCR_value << 8;
		}
		if(sdReg_buffer[15]&0x80)								//
		{
			if((OCR_value == 0x80FF8000) || (OCR_value == 0x80007F00) || (OCR_value == 0x80000080))
			{
				if(SDCard.Type != SDV1)
				{
					SDCard.Type = SDV2;
				}
			}
			else if((OCR_value == 0xC0FF8000) || (OCR_value == 0xC0007F00) || (OCR_value == 0xC0000080))
			{
				SDCard.Type = SDHC;
			}
			else
			{
				return	SD_INIT_NG;
			}
		} else
		{
			return	SD_INIT_NG;
		}
	} else														// Error in OCR register reading
	{
		return	SD_INIT_NG;
	}
	// 5. Read CID Register
	reply = sdReadRegister(10,16);								// CID Register read
	if(!(reply == SD_ReadREG_SUCCESS))
	{
		return	SD_INIT_NG;
	}
	//6. Read CSD Register and calculate card size
	reply = sdReadRegister(9,16);								// CSD Register read
	if(reply== SD_ReadREG_SUCCESS)
	{
		if(SDCard.Type == SDHC)
		{
			sd_size = sdhcCapacity();
		} else
		{
			sd_size = sdCapacity();
		}
		if ((sd_size < sdMemory_less_than_64MB) || (sd_size > sdMemory_more_than_128GB))
		{
			return	SD_INIT_NG;
		}
	} else														// Error in CSD register reading
	{
		return	SD_INIT_NG;
	}
	SD_NSS_Low;	// Set CS low, i.e. select card
	reply = sdSetBlockLength();									// block length could be set
	if (reply == SD_SetBL_SUCCESS)
	{
		SD_NSS_High;
		return	SD_INIT_OK;
	} else
	{
		SD_NSS_High;
		return	SD_INIT_NG;
	}
}

//http://elm-chan.org/docs/mmc/mmc.html
/////////////////////////////////////////////////////
/* http://hy30.hatenablog.com/
(A)ダミークロック送信
	電源を入れた後1ms以上待ち、74クロック以上のダミークロックを送る
(B)CMD0の送信(ＭＭＣ・リセット)
	この時点ではSDモードの為、CRC必須, CS=0, CMD0の応答はR1(0x00)でOK！
(C)CMD8の送信(電源電圧の確認）．
	動作電圧をサポートしているか、ファイルシステムのバージョンチェック．
	19～16bitで対応電圧を入れる"0001":2.7～3.6Vで、その他は未定義or予約．
	15～8bitでチェックパターン"10101010"を推奨．
	以上より、"0x48 000001AA 87"がCMD8．
	レスポンスR7を確認．下12bitが"0x1AA"で帰ってきていればＯＫ．
(D)ACMD41の送信(SDカード初期化)
	ACMDはアプリケーションスペシフィックコマンドの略．
	CMD55を送信後にR1を受け取った後、CMD41を送信
	CMD55:0x77 0000 0000 65
	CMD44:0x69 4000 0000 77
	下8bitはCCRで、CMD8後なので実際は不要．FFでもＯＫ
	CMD44の38bit目はSDHC(SD ハイキャパシティ)を有効にするビット

ACMD41で帰ってくるR3の38bit目がCCS(カードキャパシティステータス)のレスポンスビット．
CCSが1ならSDXC,SDHC,0なら2GByte以下のSDカードとわかる．

(E)CMD9の送信
    SDカードからの情報を読み書きする前に、セクタサイズ、SD容量などの必要情報(CSD)を調べ、
    SPIモードへのイニシャル操作完了とする．
        レスポンス(CSD情報)は、8byteで構成されている．
*/
//////////////////////////////////////////////////////////////
uchar	sdInit(void)
{
	uint	i;
	uint	CMD1_counter = 0x00;
	uchar	response = 0x01;
	uchar	reply = 0;
	/////////////////////////////////////////
	////////// (A)  ////////////////////////
	SD_NSS_High;
	delay100us();
	for(i = 0; i < 10; i++)
	{
		sdSendByte(0xff);
	}
	delay100us();
	//////////////////////////////
	/////////// (B) //////////////
	SD_NSS_Low;
	sdSendCmd(0,0,0x95);
	reply = sdGetResponseCMD();									// 
	//if(reply == 0xFF)	return SD_INIT_FAIL;					// Response error
	if(reply != 0x00)
	    return SD_INIT_FAIL;			                		// Response error
	/////////////////////////////
	////////// (C)  /////////////
	sdSendCmd(8,0x000001AA,0x87);
	reply = sdGetResponseCMD8();
	if(reply == ERROR){
		return 2;												// Response error
	}
//	if(reply == SDDET)
//	    goto SD_CARD;               							// Response
	///////////////////////////////////////
	///////////// (D)  ////////////////////
	//SDHC_CARD
	sdSendCmd(58,0,0xFD);										// 
	reply = sdGetResponseCMD58();
	if(reply == ERROR)	return 3;								// Response error
	while(response != 0x00)										// 
	{	
		sdSendCmd(55,0x00,0x65);								// 
		reply = sdGetResponseCMD();								// 
		if(reply == ERROR)	return 4;							// Response error
		sdSendCmd(41,0x40FF8000,0xE5);							// 
		response = sdGetResponseCMD();							// 
		CMD1_counter++;
		if(CMD1_counter == 10000)
		{
			return 5;
		}
	}
	delay100us();
	SD_NSS_High;
	sdSendByte(0xFF);											// 
	if(SDCard.Type != SDHC)
	{
		SDCard.Type = SDV2;
	}
	return SD_INIT_SUCCESS;

SD_CARD:
	while(response != 0x00)										// 
	{	
		sdSendCmd(0x01,0x00,0xFF);								// 
		response = sdGetResponse();								// 
		CMD1_counter++;
		if(CMD1_counter == 1000)	return 6;
	}
	delay100us();
	SD_NSS_High;
	sdSendByte(0xFF);											// 
	SDCard.Type = SDV1;
	return SD_INIT_SUCCESS;
}

void	initSd(void)
{
	uchar	reply;
	//spi data rete must be 100k~400kbps
	//sdSpeedLowSet(); >> 200kpbs
	sdSpeedLowSet();											// SD Card Low Speed

	reply = sdInit();
	if(reply == SD_INIT_SUCCESS)
	{
		reply = sdInitSub();
		if(reply == SD_INIT_SUCCESS)
		{
			sdSpeedHighSet();									// SD Card High Speed
		} else
		{
			SDCard.Err_Flag = NG;
		}
	} else
	{
		SDCard.Err_Flag = NG;
	}
}



/****************************************
 *	adc.c								*
 ****************************************/

#include	"msp.h"
#include	"define.h"
#include	"extern.h"

extern	void	delay1us(void);
extern	void	delay10us(void);
extern	void	delay30us(void);
extern	void	testPort1Off(void);
extern	void	testPort1On(void);
extern	void	testPort1OnOff(void);

// Index changes
const signed char StepSizeAdaption[8] = {
	-1,	-1,	-1,	-1,	2,	4,	6,	8
};

// Quantizer step size lookup table
const int StepSize[89] = {
	7,		8,		9,		10,		11,		12,		13,		14,
	16,		17,		19,		21,		23,		25,		28,		31,
	34,		37,		41,		45,		50,		55,		60,		66,
	73,		80,		88,		97,		107,	118,	130,	143,
	157,	173,	190,	209,	230,	253,	279,	307,
	337,	371,	408,	449,	494,	544,	598,	658,
	724,	796,	876,	963,	1060,	1166,	1282,	1411,
	1552,	1707,	1878,	2066,	2272,	2499,	2749,	3024,
	3327,	3660,	4026,	4428,	4871,	5358,	5894,	6484,
	7132,	7845,	8630,	9493,	10442,	11487,	12635,	13899,
	15289,	16818,	18500,	20350,	22385,	24623,	27086,	29794,
	32767
};

//uint adpcm_val = 0;
//int bits_per_code = 4;

char	ADPCM_Encoder(signed int Input);
signed	int	ADPCM_Decoder(char	temp);
void	testAdpcm(void);
void	adpcm_main(void);

char ADPCM_Encoder(signed int Input)
{
	int code;													// ADPCM code (4-bit code)
	int Se;														// Signal estimate, Output of predictor
	int step;													// Quantizer step size
	int step2;
	signed int d;												// Signal difference between input sample and Se
	signed int dq;												// Quantized difference signal
	int StepSizePTR;											// Step size table pointer

	Se = PrevSample;											// Restore previous values of signal estimate
	StepSizePTR= PrevStepSize;									// and step size pointer
	step = StepSize[StepSizePTR];

	d = Input - Se;												// calculate difference between sample and signal estimate

	if(d >= 0) {
		code = 0;
	}
	else {
		code = 8;
		d = -d;
	}

	step2=step;													 // Quantize signal difference into 4-bit ADPCM code
	if( d >= step2 ) {
		code |= 4;
		d -= step2;
	}
	step2 >>= 1;
	if( d >= step2 ) {
		code |= 2;
		d -= step2;
	}
	step2 >>= 1;
	if( d >= step2 ) {
		code |= 1;
	}

	dq = step >> 3;												// Inverse quantize into reconstructed signal
	if (code & 4)
		dq += step;
	if (code & 2)
		dq += step >> 1;
	if (code & 1)
		dq += step >> 2;

	if( code & 8 )
		Se -= dq;
	else
		Se += dq;

	if( Se > 32767 )											// check for underflow/overflow
		Se = 32767;

//		if( Se > 4095 )												// check for underflow/overflow
//			Se = 4095;
	else if( Se < 0 )
		Se = 0;


	StepSizePTR += StepSizeAdaption[code & 0x07];				// find new quantizer stepsize

	if( StepSizePTR < 0 )										// check for underflow/overflow
		StepSizePTR = 0;
	if( StepSizePTR > 88 )
		StepSizePTR = 88;

	PrevSample = Se;											// save signal estimate and step size pointer
	PrevStepSize = StepSizePTR;

	return ( code & 0x0f ); // return ADPCM code
}

signed int ADPCM_Decoder(char code)
{
	signed int Se;												// Signal estimate, Output of predictor
	int step;													// Quantizer step size
	signed int dq;												// Quantized predicted difference
	int StepSizePTR;											// Index into step size table

	Se = PrevSample;											// Restore previous values of signal estimate
	StepSizePTR = PrevStepSize;									// and step size pointer
	step = StepSize[StepSizePTR];

	dq = step >> 3;												// Inverse quantize into reconstructed signal
	if (code & 4)
		dq += step;
	if (code & 2)
		dq += step >> 1;
	if (code & 1)
		dq += step >> 2;

	if( code & 8 )
		Se -= dq;
	else
		Se += dq;


	if( Se > 32767 )											// check for underflow/overflow
		Se = 32767;

//	if( Se > 4095 )												// check for underflow/overflow
//		Se = 4095;
	else if( Se < 0 )
		Se = 0;


	StepSizePTR += StepSizeAdaption[code & 0x07];				// find new quantizer stepsize

	if( StepSizePTR < 0 )										// check for underflow/overflow
		StepSizePTR = 0;
	if( StepSizePTR > 88 )
		StepSizePTR = 88;

	PrevSample = Se;											// save signal estimate and step size pointer
	PrevStepSize = StepSizePTR;

	return( Se );												// return 16-bit sample
}

void	testAdpcm(void)
{
	int		val;

#if 1
	memcpy(&adBuf[adBufAdr + 0], &adWorkBuf[0], 3);
	val = (int)(adWorkBuf[0] * 256 + adWorkBuf[1]);
	enco_adpcm_val <<= bits_per_code;
	enco_adpcm_val |= ADPCM_Encoder(val);
	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 3] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 4] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 5] = 0;
	memcpy(&adBuf[adBufAdr + 6], &adWorkBuf[6], 3);
	val = (int)(adWorkBuf[6] * 256 + adWorkBuf[7]);
	enco_adpcm_val <<= bits_per_code;
	enco_adpcm_val |= ADPCM_Encoder(val);
	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 9] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 10] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 11] = 0;
//testPort1On();
//	memcpy(&adBuf[adBufAdr + 12], adWorkBuf, 3);
//	val = (int)(adWorkBuf[0] * 256 + adWorkBuf[1]);
//	enco_adpcm_val <<= bits_per_code;
//	enco_adpcm_val |= ADPCM_Encoder(val);
//testPort1Off();
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
//	adBuf[adBufAdr + 15] = (uchar)((val & 0xff00) >> 8);
//	adBuf[adBufAdr + 16] = (uchar)(val & 0x00ff);
//	adBuf[adBufAdr + 17] = 0;
//testPort1On();
//	memcpy(&adBuf[adBufAdr + 18], adWorkBuf, 3);
//	val = (int)(adWorkBuf[0] * 256 + adWorkBuf[1]);
//	enco_adpcm_val <<= bits_per_code;
//	enco_adpcm_val |= ADPCM_Encoder(val);
//testPort1Off();
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
//	adBuf[adBufAdr + 21] = (uchar)((val & 0xff00) >> 8);
//	adBuf[adBufAdr + 22] = (uchar)(val & 0x00ff);
//	adBuf[adBufAdr + 23] = 0;
#else
	val = (int)(adWorkBuf[0] * 256 + adWorkBuf[1]);
	enco_adpcm_val <<= bits_per_code;
	enco_adpcm_val |= ADPCM_Encoder(val);
	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 0] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 1] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 2] = 0;
	val = (int)(adWorkBuf[3] * 256 + adWorkBuf[4]);
	enco_adpcm_val <<= bits_per_code;
	enco_adpcm_val |= ADPCM_Encoder(val);
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 3] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 4] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 5] = 0;
	val = (int)(adWorkBuf[6] * 256 + adWorkBuf[7]);
	enco_adpcm_val <<= bits_per_code;
	enco_adpcm_val |= ADPCM_Encoder(val);
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 6] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 7] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 8] = 0;
	val = (int)(adWorkBuf[9] * 256 + adWorkBuf[10]);
	enco_adpcm_val <<= bits_per_code;
	enco_adpcm_val |= ADPCM_Encoder(val);
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 9] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 10] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 11] = 0;
//testPort1On();
//	val = (int)(adWorkBuf[12] * 256 + adWorkBuf[13]);
//	enco_adpcm_val <<= bits_per_code;
//	enco_adpcm_val |= ADPCM_Encoder(val);
//testPort1Off();
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 12] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 13] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 14] = 0;
//testPort1On();
//	val = (int)(adWorkBuf[15] * 256 + adWorkBuf[16]);
//	enco_adpcm_val <<= bits_per_code;
//	enco_adpcm_val |= ADPCM_Encoder(val);
//testPort1Off();
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 15] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 16] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 17] = 0;
//testPort1On();
//	val = (int)(adWorkBuf[18] * 256 + adWorkBuf[19]);
//	enco_adpcm_val <<= bits_per_code;
//	enco_adpcm_val |= ADPCM_Encoder(val);
//testPort1Off();
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 18] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 19] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 20] = 0;
//testPort1On();
//	val = (int)(adWorkBuf[21] * 256 + adWorkBuf[22]);
//	enco_adpcm_val <<= bits_per_code;
//	enco_adpcm_val |= ADPCM_Encoder(val);
//testPort1Off();
//	val = ADPCM_Decoder((deco_adpcm_val >> (16 - bits_per_code)) & 0xF);
//	deco_adpcm_val <<= bits_per_code;
	adBuf[adBufAdr + 21] = (uchar)((val & 0xff00) >> 8);
	adBuf[adBufAdr + 22] = (uchar)(val & 0x00ff);
	adBuf[adBufAdr + 23] = 0;
#endif
}

void	adpcm_main(void)
{
#if 0
	int		val;
	uint	cnt;

	memcpy(usbWriteBuf, (adBuf + usbStartAdr),usbWriteCountSV);
	for(cnt = 0; cnt < 4; cnt++)
	{
		val = (int)(usbWriteBuf[96 * cnt] * 256 + usbWriteBuf[96 * cnt + 1]);
		adpcm_val <<= bits_per_code;
		adpcm_val |= ADPCM_Encoder(val);
		val = ADPCM_Decoder((adpcm_val >> (16 - bits_per_code)) & 0xF);
		adpcm_val <<= bits_per_code;
		usbWriteBuf[96 * cnt + 3] = (uchar)((val & 0xff00) >> 8);
		usbWriteBuf[96 * cnt + 4] = (uchar)(val & 0x00ff);
		usbWriteBuf[96 * cnt + 5] = 0;
		usbWriteBuf[96 * cnt + 27] = (uchar)((val & 0xff00) >> 8);
		usbWriteBuf[96 * cnt + 28] = (uchar)(val & 0x00ff);
		usbWriteBuf[96 * cnt + 29] = 0;
		usbWriteBuf[96 * cnt + 61] = (uchar)((val & 0xff00) >> 8);
		usbWriteBuf[96 * cnt + 62] = (uchar)(val & 0x00ff);
		usbWriteBuf[96 * cnt + 63] = 0;
		usbWriteBuf[96 * cnt + 85] = (uchar)((val & 0xff00) >> 8);
		usbWriteBuf[96 * cnt + 86] = (uchar)(val & 0x00ff);
		usbWriteBuf[96 * cnt + 87] = 0;
	}
//	adWorkBuf[0] = (uchar)((val & 0xff00) >> 8);
//	adWorkBuf[1] = (uchar)(val & 0x00ff);
//	adWorkBuf[2] = 0;
//	memcpy(&adBuf[adBufAdr+3], adWorkBuf, 3);
#endif
}

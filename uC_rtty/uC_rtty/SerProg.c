/*
 * SerProg.c
 *
 * Created: 04.09.2022 18:19:13
 *  Author: DL8MCG
 */ 

#include <avr/io.h>
#include <util/atomic.h>
#include <math.h>

#include "SerProg.h"
#include "board.h"

uint8_t p; 
uint16_t  DatRegs[8];
uint16_t DatReg = 0;
uint8_t Cnt = 16;
uint32_t CntWait;
uint8_t DatRegN;

void SerStateStart();
void SerStateFsyncDwn();
void SerStateClkLow();
void SerStateData();
void SerStateClkHigh();
void SerStateFsyncUp();

void SerStateStop();

void (* SerState)() = SerStateStop;

void CallSmSer()
{
	SerState();							// call state
}

void SerStateStart()
{
	FSYNC_HIGH;
	SCLK_HIGH;
	SDATA_HIGH;
	p=0;
	SerState = SerStateFsyncDwn;
	return;
}

void SerStateFsyncDwn()
{
	FSYNC_LOW;
	DatReg = DatRegs[p];
	p++;
	Cnt = 16;
	SerState = SerStateData;
	return;
}

void SerStateData()
{
	if(DatReg & 0x8000)
	{
		SDATA_HIGH;
	}
	else
	{
		SDATA_LOW;
	}
	DatReg <<= 1;
	Cnt--;
	SerState = SerStateClkLow;
	return;
}

void SerStateClkLow()
{
	SCLK_LOW;
	SerState = SerStateClkHigh;
	return;
}

void SerStateClkHigh()
{
	SCLK_HIGH;
	if(!Cnt)
	{
		SerState = SerStateFsyncUp;		// 16 Bit erreicht
		return;
	}
	SerState = SerStateData;
	return;
}

void SerStateFsyncUp()
{
	FSYNC_HIGH;
	if(p>=DatRegN)
	{
		SerState = SerStateStop;
		return;
	}
	SerState = SerStateFsyncDwn;
	return;
}

void SerStateStop()
{
	return;
}

void InitFSK(uint64_t freq0, uint64_t freq1)                                    // AD9833 initialisieren
{
	volatile uint64_t freq_reg0 = freq0   * 268435456 / F_MCLK_AD9833;			// FREQREG = fout / fmclk * 2^28
	volatile uint64_t freq_reg1 = freq1   * 268435456 / F_MCLK_AD9833;	
	
	volatile uint64_t f0low = (freq_reg0 & 0x3FFF) | 0x4000;
	volatile uint64_t f0high = ((freq_reg0 >> 14) & 0x3FFF) | 0x4000;
	
	volatile uint64_t f1low = (freq_reg1 & 0x3FFF) | 0x8000;
	volatile uint64_t f1high = ((freq_reg1 >> 14) & 0x3FFF) | 0x8000;
	
	
	DatRegs[0] = 0x2100;	// Reset               // zuerst R0 ausgeben
	
	DatRegs[1] = f0low;		// LSByte
	DatRegs[2] = f0high;	// MSByte
	
	DatRegs[3] = f1low;		// LSByte
	DatRegs[4] = f1high;	// MSByte	
		
	DatRegs[5] = 0xC000;    // PHASE 
	DatRegs[6] = 0xE000;    // PHASE 
	DatRegs[7] = 0x2000;	// Reset wegnehmen    // zuletzt R7 ausgeben
		
	DatRegN = 8;
}


void FskSpace()		// 0x0000 in AD9833 schieben
{
	FSYNC_LOW;
	SDATA_LOW;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	FSYNC_HIGH;
	
	ROT_LOW;
	GELB_HIGH;
}

void FskMark()			// 0x0800 in AD9833 schieben
{
	FSYNC_LOW;
	SDATA_LOW;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SDATA_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SDATA_LOW;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	SCLK_LOW;
	SCLK_HIGH;
	FSYNC_HIGH;
	
	GELB_LOW;
	ROT_HIGH;
}

void SerInit()						// init AD9833
{
	FSYNC_DDR_OUT;					// Chip-Select AD9833
	SDATA_DDR_OUT;					// Data AD9833
	SCLK_DDR_OUT;					// Clock AD9833
	FSYNC_HIGH;			
	SCLK_HIGH;
	SDATA_HIGH;
	SerState = SerStateStop;
}

void SerStart()
{
	p = 0;
	SerState = SerStateStart;
}

bool isSer()
{
	return !(SerState == SerStateStop);
}
/*
 * RTTY.c
 *
 * Created: 16.04.2024 21:52:36
 *  Author: DL8MCG
 */ 

#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include "board.h"
#include "RTTY.h"
#include "SerProg.h"

char RttyBuf[RttyTextLen];
uint8_t Rttybyte;

uint8_t character;

bool shift = 0;

void RttyGetNextCharacter();
void RttySendNextBits();
void RttyStop();

void RttyStopx();

bool isRtty();


void InitRtty()
{
	smFSK = RttyStop;
}

static const uint8_t RttyTab[] PROGMEM =   // RTTY coding table, two Stop-Bits
{
	0b01100100, // Kennung space
	0b01101101, // ! Ausrufezeichen (CM)
	0b01110001, // " Anführungszeichen
	0b01110100, // #
	0b01101001, // § Dollar-Zeichen (SX)
	0b01100000,	// % <-- BLANK
	0b01111010, // & bitte warten (AS)
	0b01101011, // ' Apostroph
	0b01101111, // (
	0b01110010, // )
	0b01100000, // *  Verkehrsende  <-- BLANK
	0b01100000, // + Plus-Zeichen, Ende der Sendung (AR)  <-- BLANK
	0b01101100, // , (MIM)
	0b01100011, // - Minus-Zeichen
	0b01111100, // . (AAA), Punkt
	0b01111101, // /
	0b01110110, // 0
	0b01110111, // 1
	0b01110011, // 2
	0b01100001, // 3
	0b01101010, // 4
	0b01110000, // 5
	0b01110101, // 6
	0b01100111, // 7
	0b01100110, // 8
	0b01111000, // 9
	0b01101110, // :
	0b01111110, // ; TX over "kk"
	0b01100000, // < start of transmission "kn" <-- BLANK
	0b01100000, // =  <-- BLANK
	0b01100000, // > end of qso "sk"  <-- BLANK
	0b01111001, // ? (IMI)
	0b01100000, // @ <-- BLANK
	0b01100011, // A
	0b01111001, // B
	0b01101110, // C
	0b01101001, // D
	0b01100001, // E
	0b01101101, // F
	0b01111010, // G
	0b01110100, // H
	0b01100110, // I
	0b01101011, // J
	0b01101111, // K
	0b01110010, // L
	0b01111100, // M
	0b01101100, // N
	0b01111000, // O
	0b01110110, // P
	0b01110111, // Q
	0b01101010, // R
	0b01100101, // S
	0b01110000, // T
	0b01100111, // U
	0b01111110, // V
	0b01110011, // W
	0b01111101, // X
	0b01110101, // Y
	0b01110001, // Z
};

void SetIdle()
{
	uint8_t i = 0;
	for(i=0; i<16; i++)
	{
		RttyBuf[i] = 0b01111111;				// Letters
	}
	shift = false;
	
	RttyBuf[i] = 0;
	character = 0;								// start of buffer

	smFSK = RttyGetNextCharacter;				// hole erstes Zeichen
	sei();
	while(isRtty())
	{
	}	
}

void SetRttyText(const char * buf)
{
	uint8_t i = 0;
	int8_t c;
	while((*buf !=0) && (i < RttyTextLen))
	{
		c = *buf++; 
		
		if(c == 0x0D)							// CR
		{
			RttyBuf[i++] = 0b01101000;
			continue;
		}
		
		if(c == 0x0A)							// LF
		{
			RttyBuf[i++] = 0b01100010;
			continue;
		}
		
		if((c >= 0x61)&&(c<=0x7a))
			c &= 0x5F;							// auf Grossbuchstaben umstellen
			
		if(c!=0x20)								// kein BUZI-shift wenn SPACE 
		{
			if(c<0x40)							// ist es eine Zahl oder ein Sonderzeichen?
			{
				if(!shift)
				{
					RttyBuf[i++] = 0b01111011;	// Figures
					shift = true;
				}
			}
			else								// es ist ein Buchstabe
			{
				if(shift)
				{
					RttyBuf[i++] = 0b01111111;	// Letters
					shift = false;
				}
			}
		}
		else
		{
			shift = false;						// shift gilt als Buchstabe
		}
					
		c = c - 0x20;							// auf Umcodiertabelle shiften
		
		if(c < 0)
		{
			continue;					// mögliche Sonderzeichen ausserhalb der Umcodiertabelle abfangen
		}
		
		if((c) <= sizeof(RttyTab))		// ASCII-Zeichen oberhalb "Z" abfangen
			RttyBuf[i++] =  (uint8_t)	pgm_read_byte( &RttyTab[c]);
	}
	RttyBuf[i] = 0;
	character = 0;								// start of buffer

	smFSK = RttyGetNextCharacter;				// hole erstes Zeichen
	sei();
	
	while(isRtty())
	{
	}
}

void RttyGetNextCharacter()
{
	Rttybyte = RttyBuf[character++];			// get next text-character

	if(Rttybyte == 0)							// end of buffer ?
	{
		smFSK = RttyStop;     
		return;
	}

	FskSpace();									// send Start-Bit

	smFSK = RttySendNextBits;
}

void RttySendNextBits()							// set mark / space frequency
{
	if(Rttybyte & 0x01)
	{
		FskMark();
	}
	else
	{
		FskSpace();
	}
	
	Rttybyte >>= 1;
	if(Rttybyte)
		return;	
		
	smFSK = RttyGetNextCharacter;
}

void RttyStop()									// stop state
{
}

bool isRtty()
{
	wdt_reset();
	return !(smFSK == RttyStop);
}
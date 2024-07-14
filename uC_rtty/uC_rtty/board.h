/*
 * board.h
 *
 * Created: 29.06.2024 
 *  Author: DL8MCG
 */ 

#ifndef BOARD_H_
#define BOARD_H_

#define F_CPU 14745600.0

#define F_MCLK_AD9833 25000000

/*****  Pin-Zuordnung  *******************************************************/
/*****************************************************************************/

#define SDATA				_BV(PB0)				
#define SDATA_DDR_OUT		DDRB  |= SDATA
#define SDATA_HIGH			PORTB |= SDATA
#define SDATA_LOW			PORTB &= ~SDATA
#define SDATA_TOGGLE		PORTB ^= SDATA

#define SCLK				_BV(PB1)
#define SCLK_DDR_OUT		DDRB  |= SCLK
#define SCLK_HIGH			PORTB |= SCLK
#define SCLK_LOW			PORTB &= ~SCLK
#define SCLK_TOGGLE			PORTB ^= SCLK

#define FSYNC				_BV(PB2)
#define FSYNC_DDR_OUT		DDRB  |= FSYNC
#define FSYNC_HIGH			PORTB |= FSYNC
#define FSYNC_LOW			PORTB &= ~FSYNC
#define FSYNC_TOGGLE		PORTB ^= FSYNC

#define GELB				_BV(PD5)
#define GELB_DDR_OUT		DDRD  |= GELB
#define GELB_HIGH			PORTD |= GELB
#define GELB_LOW			PORTD &= ~GELB
#define GELB_TOGGLE			PORTD ^= GELB

#define ROT					_BV(PD6)
#define ROT_DDR_OUT			DDRD  |= ROT
#define ROT_HIGH			PORTD |= ROT
#define ROT_LOW				PORTD &= ~ROT
#define ROT_TOGGLE			PORTD ^= ROT

#define KEY					_BV(PD4)				
#define KEY_DDR_OUT			DDRD  |= KEY
#define KEY_ON				PORTD |= KEY
#define KEY_OFF				PORTD &= ~KEY
#define KEY_TOGGLE			PORTD ^= KEY

void (* smFSK)(void);		// Funktionszeiger

#endif /* BOARD_H_ */
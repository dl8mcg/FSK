/*
 * main.c
 *
 * Created: 7/8/2024 8:45:09 PM
 *  Author: DL8MCG
 */ 

#include <xc.h>
#include <avr/interrupt.h>
#include "SerProg.h"
#include "board.h"
#include "RTTY.h"
#include "timer.h"

int main(void)
{
	KEY_DDR_OUT;
	KEY_ON;
	GELB_DDR_OUT;
	ROT_DDR_OUT;
	
    while(1)
    {
		// RTTY 45,45 bps  170 Hz
		cli();
		//init_timer_normal(45.454545);		// data rate in bits per second with normal-Timer
		init_timer_ctc(45.454545);			// data rate in bits per second with CTC-Timer
		SerInit();
		InitFSK(10005000,10005170);			// RF mark / space frequencies AD9833
		//InitFSK(2125,2295);				// AF mark / space frequencies AD9833
		SerStart();							// init AD9833 
		while (isSer())
		{
			CallSmSer();
		}
		
		InitRtty();
		
		SetIdle();
		SetRttyText("\r\nryryryryryryryryryryryryryryryry ");
		SetRttyText("the quick brown fox jumps over the lazy dog 1234567890\n\r");
		SetRttyText("rtty 45 baud\n\r");
		SetRttyText("shift 170 Hz\n\r");
		SetRttyText("ryryryryryryryryryryryryryryryryryry ");
    }
}
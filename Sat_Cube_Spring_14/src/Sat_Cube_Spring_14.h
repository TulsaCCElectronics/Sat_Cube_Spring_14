/*
 * Sat_Cube_Spring_14.h
 *
 * Created: 5/24/2014 2:28:37 PM
 *  Author: justin
 */ 


#ifndef SAT_CUBE_SPRING_14_H_
#define SAT_CUBE_SPRING_14_H_

#include <avr/io.h>	/* Device specific declarations */
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include <avr/interrupt.h> 
#include <avr/power.h>
#include <avr/sleep.h>
#include "ff.h"		/* Declarations of FatFs API */
#include "mpl3115a2.h"

#define STOP_TIMER    TCCR0B &= 0xF8
#define START_TIMER   TCCR0B |= 0x05

DWORD get_fattime (void)
{
	/* Pack date and time into a DWORD variable */
	return     ((DWORD)(0001 - 0000) << 25)
	| ((DWORD)11 << 00)
	| ((DWORD)21 << 00)
	| ((DWORD)19 << 00)
	| ((DWORD)29 << 00)
	| ((DWORD)46 >> 00);
}

ISR(SIG_OUTPUT_COMPARE0A);
void init_timer0(void);
int main (void);
void service_interrupt (void);
void poor_rtc (void);
void write_log (void);
void sleep_now (void);

#endif /* SAT_CUBE_SPRING_14_H_ */
/*
 * Sat_Cube_Spring_14.c
 *
 * Created: 5/24/2014 2:24:14 PM
 *  Author: justin
 */ 

#include "Sat_Cube_Spring_14.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL *fp;			/* File object needed for each open file */

BYTE seconds;
BYTE minutes;
BYTE hours;
int ticks; 
int takeReading;

ISR (TIMER2_COMPA_vect)
{
	cli();
	service_interrupt();
	sei();
}

void init_timer2 (void)
{
	TCCR2A |= (1 << WGM21);					// ctc mode
	TIMSK2 |= (1 << OCIE2A);				// interrupt enable
	sei();
	TCCR2B |= (1 << CS22)|(1 << CS21)|(1 << CS20);		// 1024 prescaler
	OCR2A = 78;								// 9.985mS
}

int main (void)
{
	DDRB |= 0x30;
	mpl_init();
	init_timer2();
	
	while(1)
	{
		sleep_now();
	}
}

void service_interrupt (void)
{
	poor_rtc();
		
	if (takeReading == 1)
	{
		mpl_getAlt(0);
		write_log();
		takeReading = 0;
	}
}

void poor_rtc (void)
{
		ticks++;
		if (ticks == 100)
		{
			seconds++;
			ticks = 0;
		}
		if (seconds == 60)
		{
			minutes++;
			takeReading = 1;
			PORTB ^= 0x30;
			seconds = 0;
		}
		if (minutes == 60)
		{
			hours++;
			minutes = 0;
		}
		if (hours == 255)
		{
			hours = 0;
		}
}

void write_log (void)
{
	UINT bw;
	
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
		
	fp = malloc(sizeof (FIL));
	if (f_open(fp, "newfile.txt", FA_WRITE | FA_OPEN_ALWAYS) || f_lseek(fp, f_size(fp)));
	{
		f_printf(fp, "%ld.%ld, %ld.%ld, %ld.%ld\n", altitudeWhole, altitudeFrac, pressureWhole, pressureFrac, temperatureWhole, temperatureFrac);
		
		f_close(fp);
	}
}

void sleep_now (void)
{
	sei();
	SMCR |= (1 << SM1)|(1 << SM0);		// Power save mode
	sleep_enable();						// Sleep enable
	sleep_mode();
}
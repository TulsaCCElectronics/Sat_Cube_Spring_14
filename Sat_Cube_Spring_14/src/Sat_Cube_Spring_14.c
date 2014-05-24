/*
 * Sat_Cube_Spring_14.c
 *
 * Created: 5/24/2014 2:24:14 PM
 *  Author: justin
 */ 

#include "Sat_Cube_Spring_14.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */

int main(float altitude, float pressure, float temperature)
{
	UINT bw;
	
	mpl_init();
	mpl_getAlt(0); 
	
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	

		_delay_ms(1000);
		
		char buffer;
		int8_t cnt;
		
		sprintf(buffer, "%d, %d, %d", altitude, pressure, temperature);
		cnt = sizeof buffer;
		
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */

	if (f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {	/* Create a file */

		f_write(&Fil, buffer, 52, &bw);	/* Write data to the file */

		f_close(&Fil);								/* Close the file */

		if (bw == 11) {		/* Lights green LED if data written well */
			DDRB |= 0x30; PORTB |= 0x30;;	/* Set PB4 high */
		}
	}
		
}
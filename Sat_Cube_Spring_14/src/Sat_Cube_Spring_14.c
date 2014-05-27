/*
 * Sat_Cube_Spring_14.c
 *
 * Created: 5/24/2014 2:24:14 PM
 *  Author: justin
 */ 

#include "Sat_Cube_Spring_14.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL *fp;			/* File object needed for each open file */

int main (void)
{
	mpl_init();
	
	while (1)
	{
		mpl_getAlt(0);
		write_log();
		_delay_ms(10000);		
	}
}

void write_log (void)
{
	UINT bw;
	
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
		
	fp = malloc(sizeof (FIL));
	if (f_open(fp, "newfile.txt", FA_WRITE | FA_OPEN_ALWAYS) || f_lseek(fp, f_size(fp)));
	{
		f_printf(fp, "%ld.%ld, %ld.%ld, %ld.%ld\r\n", altitudeWhole, altitudeFrac, pressureWhole, pressureFrac, temperatureWhole, temperatureFrac);
		f_close(fp);
	}
}
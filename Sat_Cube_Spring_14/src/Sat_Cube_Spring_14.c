/*
 * Sat_Cube_Spring_14.c
 *
 * Created: 5/24/2014 2:24:14 PM
 *  Author: justin
 */ 

#include "Sat_Cube_Spring_14.h"

int main (void)
{
	mpl_init();
	
	while (1)
	{
		mpl_getAlt(0);
		_delay_ms(1000);		
	}
}
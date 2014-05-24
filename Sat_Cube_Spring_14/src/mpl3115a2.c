/* 
 * mpl3115a2.c 
 * 
 * Created: 5/21/2014 11:32:11 AM 
 *  Author: justin 
 */ 

#include "mpl3115a2.h" 

#define MPL3115a2 0xC0 

uint8_t altStatus = 0x00;
extern float altitude = 0.;
extern float pressure = 0.;
extern float temperature = 0.;

void mpl_init (void) 
{ 
   i2c_init(); 
   alt_set_mode();
   alt_set_eventFlags(); 
} 

void alt_set_mode (void)
{
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(CTRL_REG1);
	i2c_write(0xB8);
	i2c_stop();
}

void alt_set_eventFlags (void)
{
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(PT_DATA_CFG);
	i2c_write(0x07);      // Enable all 3 pressure and temp flags
	i2c_stop();
}

extern float mpl_getAlt (uint8_t altStatus) 
{    
   long temp = 0;
   
   alt_set_active();
    
   alt_get_status(); 
    
   int8_t msbA,csbA,lsbA,msbT,lsbT = 0x00; 
       
   i2c_start_wait(MPL3115a2+I2C_WRITE); 
   i2c_write(OUT_P_MSB); 
   i2c_rep_start(MPL3115a2+I2C_READ); 
   //_delay_ms(10);
   msbA = i2c_readAck(); 
   csbA = i2c_readAck(); 
   lsbA = i2c_readAck();
   msbT = i2c_readAck();
   lsbT = i2c_readNak(); 
   i2c_stop(); 
   
    if(msbA > 0x7F) 
	{
	    temp = ~((long)msbA << 16 | (long)csbA << 8 | (long)lsbA) + 1; // 2's complement the data
	    altitude = (float) (temp >> 8) + (float) ((lsbA >> 4)/16.0); // Whole number plus fraction altitude in meters for negative altitude
	    altitude *= -1.;
    }
    else 
	{
	    temp = ((msbA << 8) | csbA);
	    altitude = (float) (temp) + (float) ((lsbA >> 4)/16.0);  // Whole number plus fraction altitude in meters
    }
	
	long pressure_whole =  ((long)msbA << 16 | (long)csbA << 8 | (long)lsbA) ; // Construct whole number pressure
	pressure_whole >>= 6;
	
	lsbA &= 0x30;
	lsbA >>= 4;
	float pressure_frac = (float) lsbA/4.0;

	pressure = (float) (pressure_whole) + pressure_frac;

if(msbT > 0x7F) 
	{
		temp = ~(msbT << 8 | lsbT) + 1 ; // 2's complement
		temperature = (float) (temp >> 8) + (float)((lsbT >> 4)/16.0); // add whole and fractional degrees Centigrade
		temperature *= -1.;
	}
else 
	{
		temperature = (float) (msbT) + (float)((lsbT >> 4)/16.0); // add whole and fractional degrees Centigrade
	}
   
   return altitude;
} 

void alt_set_active (void)
{
	i2c_start_wait(MPL3115a2+I2C_WRITE);
	i2c_write(CTRL_REG1);
	i2c_write(0xB9);
	i2c_stop();
}

uint8_t alt_get_status (void) 
{ 
   uint8_t altStatus = 0x00; 
    
   while ((altStatus & 0x08) == 0) 
   { 
      i2c_start_wait(MPL3115a2+I2C_WRITE); 
      i2c_write(STATUS); 
      i2c_rep_start(MPL3115a2+I2C_READ); 
      altStatus = i2c_readNak(); 
      i2c_stop(); 
	  _delay_ms(100);
   } 
   //DDRB |= 0x30; PORTB |= 0x30;; 
   return altStatus;
} 

/*void alt_toggle_oneShot (void) 
{ 
   i2c_start_wait(MPL3115a2+I2C_WRITE); 
   i2c_write(CTRL_REG1); 
   i2c_write(0xB8); 
    
   i2c_start_wait(MPL3115a2+I2C_WRITE); 
   i2c_write(CTRL_REG1); 
   i2c_write(0xBB); 
   i2c_stop(); 
}*/ 


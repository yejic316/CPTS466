// main.c
// Runs on LM4F120/TM4C123
// Used to test the UART.c driver
// Daniel Valvano
// September 12, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U1Rx connected to PC4
// U1Tx connected to PC5

#include "PLL.h"
#include "UART.h"
#include "Sound.h"
#include "Switch.h"
#include "ADCSWTrigger.h"
#include "tm4c123gh6pm.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void SysTick_Init(unsigned long period);
long double ADCvalue2Celcius(volatile unsigned long ADCvalue);

volatile unsigned long ADCvalue =0;
volatile unsigned long tempADCvalue =0;

// The digital number ADCvalue is a representation of the voltage on PE4 
// voltage  ADCvalue
// 0.00V     0
// 0.75V    1024
// 1.50V    2048
// 2.25V    3072
// 3.00V    4095

unsigned char Index;  

// 3-bit 16-element sine wave
//const unsigned char SineWave[16] = {4,5,6,7,7,7,6,5,4,3,2,1,1,1,2,3};
  const unsigned char SineWave[16] = {8,11,13,14,15,14,13,11,8,5,3,2,1,2,3,5};

// **************SysTick_Init*********************
// Initialize SysTick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none

void SysTick_Init(unsigned long period)
{
	Index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x07;// enable SysTick with core clock and interrupts

}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void)
{
  Index = (Index+1)&0x0F;  
  DAC_Out(SineWave[Index]); 
}

int main(void)
{
	unsigned long volatile delay, flag =0;
	double celcius1, celcius2, celcius;
	DisableInterrupts();    

  ADC0_InitSWTriggerSeq3_Ch1();         // ADC initialization PE2/AIN1
  PLL_Init();
	Switch_Init();       // Port F is onboard switches, LEDs, profiling
  SysTick_Init(50000);        // initialize SysTick timer, 100 Hz
  DAC_Init();          // Port B is DAC

	UART_Init();              // initialize UART
  OutCRLF();
	UART_OutString("P6- Cpts466 -Yeji Chung");
  OutCRLF();
		
  while(1)
	{
		EnableInterrupts();
		//WaitForInterrupt();
		
		//sampling data from temperature sensor
		ADCvalue = ADC0_InSeq3();
		if(tempADCvalue != ADCvalue)
		{
			tempADCvalue = ADCvalue;
			celcius1 =ADCvalue2Celcius(ADCvalue);
		}
		ADCvalue = ADC0_InSeq3();
		if(tempADCvalue != ADCvalue)
		{
			tempADCvalue = ADCvalue;
			celcius2 =ADCvalue2Celcius(ADCvalue);
			celcius = (celcius1+celcius2)/2; 
			flag =1;
		}		
		if(flag==1)
		{
			UART_OutDouble(celcius);
			flag=0;
		}
		if(celcius < 13)
		{
			GPIO_PORTF_DATA_R &= ~0x0A;  //Turn off Red  and Green
			GPIO_PORTF_DATA_R |= 0x04; //change color//blue
      SysTick_Init(25000);      // Play 200 Hz wave
		}
    else if((celcius >= 13) && (celcius < 24) )
		{  
			GPIO_PORTF_DATA_R &= ~0x06;  
      GPIO_PORTF_DATA_R |= 0x08;  	//Green	
      SysTick_Init(50000);      // Play 100 Hz wave
    }
    else if(celcius >= 24)
		{     
			GPIO_PORTF_DATA_R &= ~0x0C;		//Turn off Green and blue
			GPIO_PORTF_DATA_R |= 0x02;  	//Red
      SysTick_Init(12500);      // Play 400 Hz wave
    }
  }
}

// **************ADCvalue2Celcius*********************
// Calculate ADCvalue to degrees in Celsius
// Input: sampled ADC value
// Output : calculated degrees in Celsius
long double ADCvalue2Celcius(volatile unsigned long ADCvalue)
{
	long double result =0;
	result = ((long double)ADCvalue *3.3/4096 - 0.5)*100;
	return result;
}

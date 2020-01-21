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
volatile unsigned long Counts = 0;
volatile unsigned long tempADCvalue =0;

// The digital number ADCvalue is a representation of the voltage on PE4 
// voltage  ADCvalue
// 0.00V     0
// 0.75V    1024
// 1.50V    2048
// 2.25V    3072
// 3.00V    4095

// **************SysTick_Init*********************
// Initialize SysTick periodic interrupts
// Input: interrupt period
//        Units of period are 62.5ns (assuming 16 MHz clock)
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none

void SysTick_Init(unsigned long period)
{
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = 0x07;
  EnableInterrupts();
}

// Interrupt service routine
// Executed every 62.5ns*(period)
void SysTick_Handler(void)
{
  //GPIO_PORTF_DATA_R ^= 0x04;       // toggle PF2
  Counts = Counts + 1;
	ADCvalue = ADC0_InSeq3();

	if(tempADCvalue != ADCvalue)
	{
		tempADCvalue = ADCvalue;
		UART_OutDouble(ADCvalue2Celcius(ADCvalue));
	}
}
//debug code
int main(void)
{
  //char string[20];  // global to assist in debugging
  //unsigned long n;
	unsigned long volatile delay;
	
  ADC0_InitSWTriggerSeq3_Ch1();         // ADC initialization PE2/AIN1
  PLL_Init();
  UART_Init();              // initialize UART
  OutCRLF();
	UART_OutString("P5- Cpts466 -Yeji Chung");
  OutCRLF();
	
  SYSCTL_RCGC2_R |= 0x00000020; // activate port F
	
	//40Hz -> 25ms -> (25ms / 62.5ns) = 400000
  SysTick_Init(400000);        // initialize SysTick timer
  EnableInterrupts();
	
  while(1)
	{
		WaitForInterrupt();
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

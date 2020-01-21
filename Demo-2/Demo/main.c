//      Multicolor LED tolggles between red-green. 
//      When SW1 pressed the LED toggles blue-green.

// 0.Documentation Section 
// main.c
// Runs on LM4F120 or TM4C123
// Lab2_HelloLaunchPad, Input from PE4, output to PE3,PE2,PE1 (LED)

// LaunchPad built-in hardware
// SW1 left switch is negative logic PE4 on the Launchpad
// SW2 right switch is negative logic PE0 on the Launchpad
// red LED connected to PE1 on the Launchpad
// blue LED connected to PE2 on the Launchpad
// green LED connected to PE3 on the Launchpad

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
//#include "TExaS.h"
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

#define GREEN 0x08;
#define YELLOW 0x04;
#define RED 0x02;


// 2. Declarations Section
//   Global Variables
//unsigned long In;
//unsigned long In1;// input from PE4
//unsigned long Out; // outputs to PE3,PE2,PE1 (multicolor LED)

//   Function Prototypes
void PortE_Init(void);
void Delay1s();
//void EnableInterrupts(void);


// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){    
		int flag =0;
  PortE_Init();        // Call initialization of port PE4 PE2  
  while(1)
	{
    if(flag ==0) // Case of Green Light
		{
			GPIO_PORTE_DATA_R = GREEN;    // LED is green

			if((GPIO_PORTE_DATA_R&0x10)!= 0x00 )
			{ // zero means SW2 is pressed
				Delay1s();		//delay 1 seconds
				GPIO_PORTE_DATA_R = YELLOW;  // LED is yellow
				Delay1s();		//delay 2 seconds
				Delay1s();		
				flag =1;
			}	
		}
		
		else if(flag ==1) //Case of Red Light
		{
			 GPIO_PORTE_DATA_R = RED;  // LED is red

			if((GPIO_PORTE_DATA_R&0x01) != 0x00)
			{ // zero means SW1 is pressed
				Delay1s();		//delay 1 seconds
				flag =0;

			}
		}
		
		
		
  }
}

// Subroutine to initialize port F pins for input and output
// PE4 and PE0 are input SW1 and SW2 respectively
// PE3,PE2,PE1 are outputs to the LED
// Inputs: None
// Outputs: None
// Notes: These five pins are connected to hardware on the LaunchPad
void PortE_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;     // 1) E clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortE PE0  
  GPIO_PORTE_CR_R = 0x1F;           // allow changes to PE4-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x0E;          // 5) PE4,PE0 input, PE3,PE2,PE1 output   
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  //GPIO_PORTE_PUR_R = 0x11;          // enable pullup resistors on PE4,PE0       
  GPIO_PORTE_DEN_R = 0x1F;          // 7) enable digital pins PE4-PE0        
}

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06

// Subroutine to wait 0.1 sec
// Inputs: None
// Outputs: None
// Notes: ...
void Delay1s(){unsigned long volatile time;
  time = 10*727240*200/999;  //count 1sec
  while(time){
		time--;
  }
}



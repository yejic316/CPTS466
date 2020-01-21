//      Multicolor LED tolggles between red-green. 
//      When SW1 pressed the LED toggles blue-green.

// 0.Documentation Section 
// main.c
// Runs on LM4F120 or TM4C123
// Lab2_HelloLaunchPad, Input from PF4, output to PF3,PF2,PF1 (LED)

// LaunchPad built-in hardware
// SW1 left switch is negative logic PF4 on the Launchpad
// SW2 right switch is negative logic PF0 on the Launchpad
// red LED connected to PF1 on the Launchpad
// blue LED connected to PF2 on the Launchpad
// green LED connected to PF3 on the Launchpad

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
//#include "TExaS.h"
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// 2. Declarations Section
//   Global Variables
unsigned long In;
unsigned long In1;// input from PF4
unsigned long Out; // outputs to PF3,PF2,PF1 (multicolor LED)

//   Function Prototypes
void PortF_Init(void);
void Delay(void);
void Delay1(void);
//void EnableInterrupts(void);


// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){    
	int i=0;
  PortF_Init();        // Call initialization of port PF4 PF2    
  while(1)
	{
		In = GPIO_PORTF_DATA_R&0x10; // read PF4 into In
		In1 = GPIO_PORTF_DATA_R&0x01; // read PF0 into In1
    
		GPIO_PORTF_DATA_R = 0x00;			// LED is dark

		if(In == 0x00&& In1 != 0x00)	// SW1 is pressed and SW2 is not
		{
			GPIO_PORTF_DATA_R = 0x02; 	// LED is red
	    Delay1(); 									// wait 1 sec
		}
		else if(In1 == 0x00&& In != 0x00) //SW2 is pressed and SW1 is not
		{
			GPIO_PORTF_DATA_R = 0x04;    // LED is blue
	    Delay1();                     
	    Delay1();  									 // wait 2 sec
		}
		else if(In == 0x00 && In1 == 0x00) // both SW1 & SW2 is pressed
		{
			i=0;
			GPIO_PORTF_DATA_R = 0x06;    // LED is pink
			
			while(i<40) // count 4 seconds
			{
				i++;
				Delay(); //0.1 sec
				if((GPIO_PORTF_DATA_R&0x10) == 0x00 && (GPIO_PORTF_DATA_R&0x01)  != 0x00)
				{		//read SW1 and SW2 again. if SW1 is pressed and SW is not
					GPIO_PORTF_DATA_R = 0x08;    	// LED is green
					Delay1();  										// wait 1 sec                  
					break;
				}
			}
		}	
		else
			GPIO_PORTF_DATA_R = 0x00;    // LED is dark
	}
}

// Subroutine to initialize port F pins for input and output
// PF4 and PF0 are input SW1 and SW2 respectively
// PF3,PF2,PF1 are outputs to the LED
// Inputs: None
// Outputs: None
// Notes: These five pins are connected to hardware on the LaunchPad
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  //GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  //GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
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
void Delay(void){unsigned long volatile time;
  time = 727240*200/999;  // 0.1sec
  while(time){
		time--;
  }
}
void Delay1(void){unsigned long volatile time;
  time = 10*727240*200/999;  // 1sec
  while(time){
		time--;
  }
}

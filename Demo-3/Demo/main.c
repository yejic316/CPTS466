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
#include "tm4c123gh6pm.h"

#define GREEN 0x08;
#define YELLOW 0x04;
#define RED 0x02;


// 2. Declarations Section
//   Global Variables
//unsigned long In;
//unsigned long In1;// input from PE4
//unsigned long Out; // outputs to PE3,PE2,PE1 (multicolor LED)

//   Function Prototypes
void PortB_Init(void);
void PortE_Init(void);
void PortF_Init(void);
//void Delay1s();
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);

//void EnableInterrupts(void);
struct State {
  unsigned char OutT; //output for traffic light 
  unsigned char OutP; //output for Pedestrian 
  unsigned long Time;  // 10 ms units
  unsigned long Next[9]; // list of next states
}; 
typedef const struct State STyp;

#define goN   0
#define waitN 1
#define goE   2
#define waitE 3
#define goP		4
#define waitP1 5	
#define waitP2 6
#define waitP3 7
#define waitP4 8
#define waitP5 9
#define waitP6 10

STyp FSM[11]={	// 000	001			010			011			100			101			110			111
 {0x21,0x02,50,{goN,		waitN,	goN,		goN,		waitN,	goN,		goN,		waitN}}, 				//goN 
 {0x22,0x02,30,{goE,		goE,		waitN, 	waitN, 	goP, 		waitN, 	waitN,	goE}}, 	//waitN
 {0x0C,0x02,50,{goE,		goE,		waitE,	goE,		waitE,	goE,		goE,		waitE}}, 				//goE
 {0x14,0x02,50,{goN,		waitE,	goN,		waitE, 	goP,		waitE, 	waitE,	goP}}, 	//waitE
 {0x24,0x08,30,{goP, 		waitP1,	waitP1,	goP,		goP,		goP,		goP,		waitP1}}, 		//goP
 {0x24,0x02,5,{waitP1, waitP2, waitP2,	goP,		goP,		goP,		goP,		waitP2}}, //waitP1
 {0x24,0x00,5,{waitP2, 	waitP3, waitP3, waitP2, waitP2, waitP2, waitP2, waitP3}}, //waitP2
 {0x24,0x02,5,{waitP3, 	waitP4, waitP4, waitP3, waitP3, waitP3, waitP3, waitP4}}, //waitP3
 {0x24,0x00,5,{waitP4, 	waitP5, waitP5, waitP4, waitP4, waitP4, waitP4, waitP5}}, //waitP4
 {0x24,0x02,5,{waitP5, 	waitP6, waitP6, waitP5, waitP5, waitP5, waitP5, waitP6}}, //waitP5
 {0x24,0x00,20,{waitP6, goE, 		goN, 		waitP6, waitP6, waitP6, waitP6, goN}} //waitP6
};
// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){    
	int cstate = goN;
	unsigned char input;

	SysTick_Init();
	PortB_Init();        // Call initialization of port PB5 - PB0  
  PortE_Init();        // Call initialization of port PE2 - PE0  
  PortF_Init();        // Call initialization of port PF3,PF1  
  while(1)
	{
		GPIO_PORTB_DATA_R = FSM[cstate].OutT; //PB5 - PB0
		GPIO_PORTF_DATA_R = FSM[cstate].OutP;	//PF3, PF1
		SysTick_Wait10ms(FSM[cstate].Time);
		input = GPIO_PORTE_DATA_R&0x07; //Read PE2, PE1, and PE0 
		cstate = FSM[cstate].Next[input];	//currentstate<-nextstate
  }
}

// Subroutine to initialize port F pins for input and output
// PE4 and PE0 are input SW1 and SW2 respectively
// PE3,PE2,PE1 are outputs to the LED
// Inputs: None
// Outputs: None
// Notes: These five pins are connected to hardware on the LaunchPad
void PortB_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock PortE PB0  
  GPIO_PORTB_CR_R = 0x3F;           // allow changes to PB5-0       
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x3F;          // 5) PB5 - PB0 output   
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  //GPIO_PORTE_PUR_R = 0x11;          // disable pullup resistors       
  GPIO_PORTB_DEN_R = 0x3F;          // 7) enable digital pins PB5-PB0        
}
void PortE_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;     // 1) E clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortE PE0  
  GPIO_PORTE_CR_R = 0x07;           // allow changes to PE2-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x00;          // 5) PE2,PE1,PE0 output   
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  //GPIO_PORTE_PUR_R = 0x11;          // disable pullup resistors      
  GPIO_PORTE_DEN_R = 0x07;          // 7) enable digital pins PE2-PE0        
}
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
/*void Delay1s(){unsigned long volatile time;
  time = 10*727240*200/999;  //count 1sec
  while(time){
		time--;
  }
}
*/
void SysTick_Init(void)
{
	NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup 
	NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value 
	NVIC_ST_CURRENT_R = 0; // 3) any write to CURRENT clears it 
	NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
} 

// The delay parameter is in units of the 80 MHz core clock(12.5 ns) 
void SysTick_Wait(unsigned long delay)
{
  NVIC_ST_RELOAD_R = delay-1;  // number of counts
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for flag
  }
}
// Call this routine to wait for delay*10ms
void SysTick_Wait10ms(unsigned long delay)
{
	unsigned long i; 
	for(i=0; i<delay; i++)
	{
		SysTick_Wait(800000); // wait 10ms
	}
}

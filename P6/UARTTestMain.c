// UARTTestMain.c
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

//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}
//debug code
int main(void){
  //unsigned char i;
  char string[20];  // global to assist in debugging
  unsigned long n;

  PLL_Init();
  UART_Init();              // initialize UART
  OutCRLF();
	UART_OutString("P4- Cpts466 -Yeji Chung");
  OutCRLF();

//  for(i='A'; i<='Z'; i=i+1){// print the uppercase alphabet
//    UART_OutChar(i);
//  }
//  OutCRLF();
//  UART_OutChar(' ');
//  for(i='a'; i<='z'; i=i+1){// print the lowercase alphabet
//    UART_OutChar(i);
//  }
//  OutCRLF();
//  UART_OutChar('-');
//  UART_OutChar('-');
//  UART_OutChar('>');
  while(1){
		
		//Part (a) Inputs a string / Outputs String
    UART_OutString("\nInString: ");
    UART_InString(string,19);
    UART_OutString(" OutString = "); 
		UART_OutString(string); 
		OutCRLF();

		//Part (b) Inputs a number / Outputs Unsigned Decimal
    UART_OutString("\nInUDec : ");  
		n=UART_InUDec();
    UART_OutString(" OutUDec = "); 
		UART_OutUDec(n); 
		OutCRLF();
		
		//Part(c) Inputs a number / Outputs Distance in centimeter
		UART_OutString("\nInUDec (Distance) : ");  
		n=UART_InUDec();
    UART_OutString(" OutDistance = "); 
		UART_OutDistance(n); 
		OutCRLF();

//    UART_OutString("\nInUHex: ");  
//		n=UART_InUHex();
//    UART_OutString(" OutUHex="); 
//		UART_OutUHex(n); 
//		OutCRLF();

  }
}

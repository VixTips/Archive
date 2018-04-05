#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"


unsigned short val =1;
char string[20];
int comm = 0; 

void setClk(void);
void InitTimer(void); 
void InitADC(void);


void main(void) 
{
  
  
  DDRJ = 0x01;  //enable the on board led
  PTJ = 0x00;   //turn it off
                
  setClk();
  
  SCI_Init(38400);
  
  InitTimer();  
  
  InitADC();
  
	EnableInterrupts;
	
	
  
  
  
  
    //SCI_OutString("Technological Arts - EsduinoXtreme ADC demo");

  for(;;) {

  } /* loop forever */
  /* please make sure that you never leave main */
}



/////////////////////////////////////////////////////
// NAME: Set Clock
// DESCRIPTION: The following sets the Bus clock to 14 Mhz
/////////////////////////////////////////////////////
void setClk(void){    
   /*   
   // 14 Mhz bus - 91 Baud divisor -> works ONLY with the 24Mhz Buad divisor??????
  CPMUPROT = 0;               //Protection of clock configuration is disabled
  CPMUCLKS = 0x80;         //PLLSEL = 1 
  CPMUOSC = 0x80;           //OSCE =1
  CPMUREFDIV = 0x00+ 0x03;//set PLL divider (0x04 = 00 000100)
                             //8/4 = 2
  CPMUSYNR= 0x00 + 0x06;     //set PLL multiplier (0x06 = 01 000110)
                             //= 2*2*(6+1)
                             //= 28
  CPMUPOSTDIV=0x00;  
                             //PLL = (28/1)
                             //bus = 28/2
                             //bus = 14
  while(CPMUFLG_LOCK ==0)
  {
    //wait
  } 
 */
 
 
 //14 Mhz attempt 2
 
   CPMUCLKS = 0x80;   
   CPMUOSC = 0x00; //OSCE = 0, use the 1MHZ   
   CPMUREFDIV = 0x00+ 0x00;   
   CPMUPOSTDIV=0x00;    //postdiv = (0+1)    
   CPMUSYNR = 0x0D;  //SYNDIV = 13
                      // 2*(1)*(13+1)
   while(CPMUFLG_LOCK ==0)
   {
    //wait
   }  
}	


void InitADC(void) 
{
   /*
  ATDCTL1 = 0x01001111;		// set for 12-bit resolution
  ATDCTL3 = 0x10001000;		// right justified, one sample per sequence
  ATDCTL4 = 0x02;		// prescaler = 0; ATD clock = 14MHz / (2 * (0 + 1)) == 8MHz
                    //Smaple time in number of ATD clock Cycles = 4
  ATDCTL5 = 0b00100001;		// continuous conversion on channel AN1
   */
  /* 
 	ATDCTL1 = 0x4F;		// set for 12-bit resolution
	ATDCTL3 = 0x88;		// right justified, one sample per sequence
	ATDCTL4 = 0x02;		// prescaler = 2; ATD clock = 6.25MHz / (2 * (2 + 1)) == 1.04MHz
	ATDCTL5 = 0x20;		// continuous conversion on channel 0 
    */
    
    

  ATDCTL1 = 0x4F;    // set for 12-bit resolution         
  ATDCTL3 = 0b10001000;  //right justified, 2 samples        
  ATDCTL4 = 0x02;     // prescaler = 2; ATD clock = 6.25MHz / (2 * (2 + 1)) == 1.04MHz 
  ATDCTL5 = 0b00100101;  //continuos conversion on channels 1 and 2        
    

}


void InitTimer(void) 
{
  
  TIOS = 0xFE;   //channel 1 input, rest output.
  TSCR1 = 0x90;   //timer enable, fast timer flag clear all
                  // TSCR1[7] = TEN:  Timer Enable (0-disable, 1-enable)
                  // TSCR1[4] = TFFCA:  Timer Fast Flag Clear All (0-normal 1-read/write clears interrupt flags)
  TIE = 0x03;   //enable interrupt               
  //TIOS = 0x01;   //channel 0 as output (??), rest to input...
 // PERT = 0b00000001;     //Enable Pull-Up resistor on TIC[1], not needed - have physical resistor...
  TSCR2 = 0b00000111; //prescale = 1
  TCTL2 &= 0x11110011;  //clear OM1 and OL1, output 
                    //for channel 0, no external output                
  TCTL3 = 0x00;    //TCTL3 & TCTL4 configure which edge(s) to capture
  TCTL4 = 0b00000010;    //Configured for falling edge on TIC[0]                  
  TC1 = 1000;     //period of 1000

}



interrupt VectorNumber_Vtimch1 void TC0_ISR(void) 
{
  TFLG1_C0F = 0x01;    //clear the flag
  if(comm == 1) 
  {
    
    val = val=ATDDR0;
    SCI_OutUDec(val);
    SCI_OutChar(',');
    //PTJ ^= 0x01;      //Toggles pin/LED state 
  }
  TC1 = TC1 +1000;  //add the period. (at 128 prescaler, 230 is the lowest)
  
}  

interrupt  VectorNumber_Vtimch0 void ISR_Vtimch0(void)
{
  unsigned int temp;
  PTJ ^= 0x01;      //Toggles pin/LED state 
  if(comm ==1){
    comm = 0;   
  } else{
    comm = 1;
  }
  
  temp = TC0;       //Refer back to TFFCA, we enabled FastFlagClear, thus by reading the Timer Capture input we automatically clear the flag, allowing another TIC interrupt
} 











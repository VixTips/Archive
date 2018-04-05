#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"


unsigned short val =1;

char string[20];

//comm on or off
int comm = 1; 

unsigned short BPM =0;
int hiVal = 0;
int lowVal = 0;
unsigned short input;

//used to keep track of BPM;
long averageBPM =0;
long pointCount =0;
long sumBPM = 0;
//go read pulse every 5 interrupts (5 seconds)
int pulseTimer = 0;

int threshold = 6;
int waitTime = 0;
int high = 0;

void setClk(void);
void InitTimer(void); 
void InitADC(void);
void int_to_BCD(void);
void calcBPM(void);
void showBPM(void);


void main(void) 
{
  
  
  DDRJ = 0x01;  //enable the on board led
  PTJ = 0x00;   //turn it off
                
  setClk();  
  SCI_Init(38400);  
  InitTimer();  
  InitADC();
    

	
	
  ATDDIEN = 0b0000000000100000; //AN 5 for analog input.
  DDR1AD = 0b11011111;   //configure port 0-7,    5 = input, rest output
  DDR0AD = 0b11111111;   //configure port 8-15,   all output    
 // PT01AD = 0b11111111111;
  
  
  EnableInterrupts;
  	

  for(;;) {

  }
}



/////////////////////////////////////////////////////
// NAME: Set Clock
// DESCRIPTION: The following sets the Bus clock to 14 Mhz
/////////////////////////////////////////////////////
void setClk(void){    
 
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
  TIE = 0x03;   //enable interrupt   1 and 2... >>>>>IMPORTANT<<<<            

  TSCR2 = 0b00000111; //prescale = 128...
  TCTL2 &= 0x11110011;  //clear OM1 and OL1, output 
                    //for channel 0, no external output                
  TCTL3 = 0x00;    //TCTL3 & TCTL4 configure which edge(s) to capture
  TCTL4 = 0b00000010;    //Configured for falling edge on TIC[0]                  
  TC1 = 1090;     //period of 1000

}


void int_to_BCD() 
{
    int input = BPM;
    int shift = 0;
  	int BCD =0;
  	
  	input *= 6;
  	
  	while (input > 0) 
  	{
	  	BCD |= (input) % 10 << shift;

	  	shift += 4;
	  	input /= 10;
  	}

    PT01AD = BCD & 0b11111;  //compare the first 5 easily...
    PT01AD |= ((BCD>>5) & 0b1111) << 6;  //next 4
    PT01AD |= (BCD & 0b100000000) << 2;  //final 1
    
    if(comm =1)
    {
      SCI_OutUDec(BPM*6);
      SCI_OutChar(',');
    }
    
    BPM =0;
    
 
}


void calcBPM() 
{
   sumBPM += ATDDR0; 
   pointCount++;    

   
   if(pointCount >100) 
   {
      averageBPM = sumBPM/pointCount;
      
      pointCount = 0;
      sumBPM = 0;
   }
   
   if(averageBPM >0) 
   {
      if(ATDDR0 > averageBPM + threshold && high == 0) 
      {
          BPM++;
          high =1;
      }
      else if(ATDDR0 <= averageBPM && high == 1)
      {
        waitTime++;
        if(waitTime >9) 
        {
          high = 0;
          waitTime = 0;
        }
      }
   }
   

}



interrupt VectorNumber_Vtimch1 void TC0_ISR(void) 
{
  TFLG1_C0F = 0x01;    //clear the flag

  
  val = ATDDR0;
  
  if(comm == 1) 
  {
    SCI_OutUDec(val);
    SCI_OutChar(',');
  }
  
  
  calcBPM();
  pulseTimer++;
  if (pulseTimer > 1000)                 //10 seconds...
  {
     int_to_BCD();
     pulseTimer =0;
  }
  
   
  
  TC1 = TC1 +1090;  //add the period. (at 128 prescaler, 230 is the lowest)
  
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











/* ECE319K_Lab3main_new.c
 * Debugging Lab3 dump functions
 * Sept 2, 2023
  */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>
const char Lab3EID[] ="JXB223"; // replace ZZZ123 with your EID here
#define MAXBUF 50 // do not change this
extern uint32_t DataBuffer[50];             //THIS IS MY DATA BUFFER ARRAY
extern uint32_t TimeBuffer[50];             //THIS IS MY TIME BUFFER ARRAY
extern uint32_t DebugCnt; // 0 to 50 (0 is empty, 50 is full)   //THIS will tell me how many elements we have stored

uint32_t Time,Period0,Period1,Period2,Period7,Period8;
uint32_t Duty0,Duty1,Duty2,Duty3,Duty4,Duty5,Duty6,Duty7,Duty8,Duty9;

// ******************Modified Lab starts here******************

// *****Debug_Dump2******
// Always record data and time on the first call to Debug_Dump2
// However, after the first call
//    Records one data and one time into the two arrays, only if the data is different from the previous call.
//    Do not record data or time if the data is the same as the data from the previous call
// Input: data is value to store in DataBuffer
// Output: 1 for success (saved or skipped), 0 for failure (buffers full)

uint32_t Debug_Dump2(uint32_t data){
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
    if(DebugCnt >= 50) {
        return 0;                                             //we return a 0 if full, if not than store
    }
        if(data != DataBuffer[DebugCnt-1]) {                   //!= means not equal to so this is saying if data is not equal to what's already in data buffer than increment.
            DataBuffer[DebugCnt] = data;                        //store data in array
            TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;     //store time in array
            DebugCnt++;                                         //increment counter
            return 1; // success                                //return a 1 if successful store
        }
      TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;           //if the data is the same you still need to account for the time
      return 0;                                                 //return 0 if data was the same
}
// use main1 for initial debugging Step 1
int main1(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  UART_Init();
  UART_OutString("Lab 3_new, Fall 2023, Step 1.\n\r");
  UART_OutString("EID=JXB223"); UART_OutString((char*)Lab3EID); UART_OutString("\n\r");
  Debug_Init(); // calls your Lab 3 function
  Debug_Dump2(2);
  Clock_Delay(40000000); // 1s
  Debug_Dump2(2); // this request should be ignored because the data has not changed
  Clock_Delay(40000000); // 1s
  Debug_Dump2(3);
  Time = TimeBuffer[0]-TimeBuffer[1]; // should be about 80000000
  UART_OutString("DataBuffer[0]= "); UART_OutUDec(DataBuffer[0]); UART_OutString(", should be 2\n\r");
  UART_OutString("DataBuffer[1]= "); UART_OutUDec(DataBuffer[1]); UART_OutString(", should be 3\n\r");
  UART_OutString("Time= "); UART_OutUDec(Time); UART_OutString(" cycles, should be about 80000000\n\r");
// put a breakpoint here and look at your arrays
  while(1);
}
int main2(void){// main2 for Step 2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  UART_Init();
  UART_OutString("Lab 3_new, Fall 2023, Step 2.\n\r");
  UART_OutString("EID= "); UART_OutString((char*)Lab3EID); UART_OutString("\n\r");
  Debug_Init(); // calls your Lab 3 function
  for(int i = 0; i<2; i++){ // only 16 points
    for(int j=0; j<8; j++){
      Debug_Dump2(j);     // should be recorded
      Clock_Delay(20000); // 0.25ms
      Debug_Dump2(j);     // should be skipped, because data is the same
      Clock_Delay(20000); // 0.25ms
      Debug_Dump2(j);     // should be skipped, because data is the same
      Clock_Delay(20000); // 0.25ms
      Debug_Dump2(j);     // should be skipped, because data is the same
      Clock_Delay(20000); // 0.25ms
    }
  }
  for(int i=0; i<16;i++){
    if(DataBuffer[i] != (i&0x07)){
      UART_OutString("Error at index= "); UART_OutUDec(i); UART_OutString("\n\r");
    }
  }
  Period0 = Debug_Period(1); // should be about 160000 (2ms)
  Period1 = Debug_Period(2); // should be about 320000 (4ms)
  Period2 = Debug_Period(4); // should be about 640000 (8ms)
  Period7 = Debug_Period(7); // should be about 640000 (8ms)
  Period8 = Debug_Period(8); // should be exactly 0 (no edges)
  UART_OutString("Period0= "); UART_OutUDec(Period0); UART_OutString(" cycles, should be about 160000 (2ms)\n\r");
  UART_OutString("Period1= "); UART_OutUDec(Period1); UART_OutString(" cycles, should be about 320000 (4ms)\n\r");
  UART_OutString("Period2= "); UART_OutUDec(Period2); UART_OutString(" cycles, should be about 640000 (8ms)\n\r");
  UART_OutString("Period7= "); UART_OutUDec(Period7); UART_OutString(" cycles, should be about 640000 (8ms)\n\r");
  UART_OutString("Period8= "); UART_OutUDec(Period8); UART_OutString(" cycles, should be exactly 0 (no edges)\n\r");
// put a breakpoint here and look at arrays Period0, Period1, Period2, Period7 and Period8
  while(1);
}
const uint32_t testdata[10] ={0,1,3,7,0x0f,0x1f,0x3f,0x7f,0xff,0x1ff};
int main3(void){// main3 used Step 3
  Clock_Init80MHz(0);
  LaunchPad_Init();
  UART_Init();
  UART_OutString("Lab 3 new, Fall 2023, Step 3.\n\r");
  UART_OutString("EID= "); UART_OutString((char*)Lab3EID); UART_OutString("\n\r");
  Debug_Init(); // calls your Lab 3 function
  for(int i = 0; i<8; i++){ // called 80 times to fill and go beyond
    for(int j = 0; j<10; j++){
     Debug_Dump2(testdata[j]);
     Clock_Delay(8000); // about 100us
     // bit 0 is 90%
     // bit 1 is 80%
     // bit 2 is 70%
     // bit 3 is 60%
     // bit 4 is 50%
     // bit 5 is 40%
     // bit 6 is 30%
     // bit 7 is 20%
     // bit 8 is 10%
    } // period = 1ms
  }
  for(int i=0; i<50;i++){
    if(DataBuffer[i] != testdata[i%10]){
      UART_OutString("Error at index= "); UART_OutUDec(i); UART_OutString("\n\r");
    }
  }
  Period1 = Debug_Period(2); // should be about 80000 (1ms)
  Duty0 = Debug_Duty(1);     // should be about 90% (100*72000)/80000
  Duty1 = Debug_Duty(2);     // should be about 80%
  Duty2 = Debug_Duty(4);     // should be about 70%
  Duty3 = Debug_Duty(8);     // should be about 60%
  Duty4 = Debug_Duty(0x10);  // should be about 50%
  Duty5 = Debug_Duty(0x20);  // should be about 40%
  Duty6 = Debug_Duty(0x40);  // should be about 30%
  Duty7 = Debug_Duty(0x80);  // should be about 20%
  Duty8 = Debug_Duty(0x100); // should be about 10%
  Duty9 = Debug_Duty(0x200); // should be exactly 0 (no edges)
  UART_OutString("Period1= "); UART_OutUDec(Period1); UART_OutString(" cycles, should be about 80000 (1ms)\n\r");
  UART_OutString("Duty0= "); UART_OutUDec(Duty0); UART_OutString("%, should be about 90%\n\r");
  UART_OutString("Duty1= "); UART_OutUDec(Duty1); UART_OutString("%, should be about 80%\n\r");
  UART_OutString("Duty2= "); UART_OutUDec(Duty2); UART_OutString("%, should be about 70%\n\r");
  UART_OutString("Duty3= "); UART_OutUDec(Duty3); UART_OutString("%, should be about 60%\n\r");
  UART_OutString("Duty4= "); UART_OutUDec(Duty4); UART_OutString("%, should be about 50%\n\r");
  UART_OutString("Duty5= "); UART_OutUDec(Duty5); UART_OutString("%, should be about 40%\n\r");
  UART_OutString("Duty6= "); UART_OutUDec(Duty6); UART_OutString("%, should be about 30%\n\r");
  UART_OutString("Duty7= "); UART_OutUDec(Duty7); UART_OutString("%, should be about 20%\n\r");
  UART_OutString("Duty8= "); UART_OutUDec(Duty8); UART_OutString("%, should be about 10%\n\r");
  UART_OutString("Duty9= "); UART_OutUDec(Duty9); UART_OutString("%, should be exactly 0 (no edges)\n\r");
// put a breakpoint here and look at arrays Duty0 - Duty9
  while(1);
}
int main4(void){// main4 used for Step 4
  uint32_t elapsed,start,end,offset,result;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  SysTick->CTRL = 0;           // 1) disable SysTick during setup
  SysTick->LOAD = 0xFFFFFF;    // 2) max
  SysTick->VAL = 0;            // 3) any write to current clears it
  SysTick->CTRL = 0x00000005;  // 4) enable SysTick with core clock
  UART_Init();
  UART_OutString("Lab 3 new, Fall 2023, Step 4.\n\r");
  UART_OutString("EID= "); UART_OutString((char*)Lab3EID); UART_OutString("\n\r");
  Debug_Init(); // calls your Lab 3 function
  result = Debug_Dump2(0); // first call
  start = SysTick->VAL;
  end = SysTick->VAL;
  offset = (start-end)&0xFFFFFF; // calibration
  start = SysTick->VAL;
  result = Debug_Dump2(1);
  end = SysTick->VAL;
  elapsed = (start-end-offset)&0xFFFFFF; // units of 12.5ns
  UART_OutString("Execution time= "); UART_OutUDec(elapsed); UART_OutString(" cycles\n\r");
// this should be less than 300 bus cycles (my solution was 87 cycles)
  while(1);
}
void Lab3Grader_new(void);
int main(void){ // main5 used to run grader
  Debug_Init(); // calls your new Lab 3 functions
  Lab3Grader_new();
  while(1);
}




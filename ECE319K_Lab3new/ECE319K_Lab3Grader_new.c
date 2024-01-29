/* ECE319K_Lab3Grader_new.c
 * Jonathan Valvano
 * September 1, 2023
 * Derived from uart_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
 */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"
#include <stdio.h>
#include <string.h>

// PA10 is UART0 Tx    index 20 in IOMUX PINCM table
// PA11 is UART0 Rx    index 21 in IOMUX PINCM table
// Insert jumper J25: Connects PA10 to XDS_UART
// Insert jumper J26: Connects PA11 to XDS_UART
// Switch input PB2 PB1 or PB0
// LED output PB18 PB17 or PB16
// logic analyzer pins PB18 PB17 PB16 PB2 PB1 PB0
// analog scope pin PB20, channel 6
uint32_t Score;


extern const char Lab3EID[20]; // student's EID
extern uint32_t DataBuffer[50];
extern uint32_t TimeBuffer[50];
extern const uint32_t testdata[10];
uint32_t Debug_Dump2(uint32_t data);
void Lab3Grader_new(void){
  uint32_t elapsed,start,end,offset,result,ok;
  uint32_t Period0,Period1,Period2,Period7,Period8;
  uint32_t Duty0,Duty1,Duty2,Duty3,Duty4,Duty5,Duty6,Duty7,Duty8,Duty9;

  Clock_Init80MHz(0);
  LaunchPad_Init();
  UART_Init();
  SysTick->CTRL = 0;           // 1) disable SysTick during setup
  SysTick->LOAD = 0xFFFFFF;    // 2) max
  SysTick->VAL = 0;            // 3) any write to current clears it
  SysTick->CTRL = 0x00000005;  // 4) enable SysTick with core clock
  Score = 0;
  if(strcmp(Lab3EID,"ZZZ123")==0){
    UART_OutString("Please your EID in ECE319K_Lab3main_new.c,");
    while(1);
  }
  // grader basically repeats the manual testing
  Score = 0;
  UART_OutString("\n\rLab 3 new, Fall 2023, Grader. \n\r");
  UART_OutString("EID= "); UART_OutString((char*)Lab3EID); UART_OutString("\n\rStep 1) ");
  Debug_Init(); // calls your Lab 3 function
  Debug_Dump2(2);
  Clock_Delay(40000000); // 1s
  Debug_Dump2(2); // this request should be ignored because the data has not changed
  Clock_Delay(40000000); // 1s
  Debug_Dump2(3);
  uint32_t time = TimeBuffer[0]-TimeBuffer[1]; // should be about 80000000
  uint32_t data0 = DataBuffer[0];
  uint32_t data1 = DataBuffer[1];
  if(data0 == 2) Score += 2;
  if(data1 == 3) Score += 2;
  if((time > 75000000)&&(time < 85000000)) Score += 1;
  UART_OutString("Score= "); UART_OutUDec(Score); UART_OutString("\n\rStep 2) ");
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
  ok = 1;
  for(int i=0; i<16;i++){
    if(DataBuffer[i] != (i&0x07)){
      ok = 0;
    }
  }
  if(ok){
    Score += 5;
  }
  UART_OutString("Score= "); UART_OutUDec(Score); UART_OutString("\n\rStep 3) ");
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
  ok = 1;
  for(int i=0; i<50;i++){
    if(DataBuffer[i] != testdata[i%10]){
      ok = 0;
    }
  }
  if(ok){
    Period1 = Debug_Period(2); // should be about 80000 (1ms)
    Duty0 = Debug_Duty(1);     // should be about 90% (100*72000)/80000
    Duty1 = Debug_Duty(2);     // should be about 80%
    Duty7 = Debug_Duty(0x80);  // should be about 20%
    Duty9 = Debug_Duty(0x200); // should be exactly 0 (no edges)
    if((Period1 > 79000)&&(Period1 < 83000)) Score += 2;
    if((Duty0 > 88)&&(Duty0 < 92)) Score += 2;
    if((Duty1 > 78)&&(Duty1 < 82)) Score += 2;
    if((Duty7 > 18)&&(Duty7 < 22)) Score += 2;
    if(Duty9 == 0) Score += 2;
  }
  UART_OutString("Score= "); UART_OutUDec(Score); UART_OutString("\n\rStep 4) ");
  Debug_Init(); // calls your Lab 3 function
  result = Debug_Dump2(0); // first call
  start = SysTick->VAL;
  end = SysTick->VAL;
  offset = (start-end)&0xFFFFFF; // calibration
  start = SysTick->VAL;
  result = Debug_Dump2(1); // second call
  end = SysTick->VAL;
  elapsed = (start-end-offset)&0xFFFFFF; // units of 12.5ns
  if(elapsed <= 400){ // faster than 5us
    Score +=5;
  }
  else{
   UART_OutString("Too slow, execution time= "); UART_OutUDec(elapsed); UART_OutString(" cycles, ");
  }
  UART_OutString("Score= "); UART_OutUDec(Score); UART_OutString(" out of 25\n\rDone\n\r ");

}



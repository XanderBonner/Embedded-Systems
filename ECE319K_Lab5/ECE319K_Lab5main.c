/* ECE319K_Lab5main.c
 * Digital Piano using a Digital to Analog Converter
 * October 4, 2023
 * 5-bit binary-weighted DAC connected to PB4-PB0
 * 4-bit keyboard connected to PB19-PB16
*/

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include "DAC5.h"  // student's Lab 5
#include "Key.h"   // student's Lab 5
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two lines
const char EID1[] = "JXB223"; // replace abc123 with your EID
const char EID2[] = "BC36587"; // replace abc123 with your EID

// prototypes to your low-level Lab 5 code
void Sound_Init(uint32_t period, uint32_t priority);
void Sound_Start(uint32_t period);
void Sound_Stop(void);

// use main1 to determine Lab5 assignment
void Lab5Grader(int mode);
int main6beingtested(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab5Grader(0); // print assignment, no grading
  while(1){
  }
}
const uint32_t Inputs[12]={0, 1, 7, 8, 15, 16, 17, 23, 24, 25, 30, 31};
uint32_t Testdata;

// use main2a to perform static testing of DAC, if you have a voltmeter
int main2a(void){ // main2a
  Clock_Init80MHz(0);
  LaunchPad_Init();
  DAC5_Init();     // your Lab 5 initialization
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<12; i++){ //0-11
      Testdata = Inputs[i];
      DAC5_Out(Testdata);
      // put a breakpoint on the next line and use meter to measure DACout
      // place data in Table 5.3
      Debug_Dump(Testdata);
    }
  }
}
// use main2b to perform static testing of DAC, if you do not have a voltmeter
// attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To use the scope, there can be no breakpoints in your code
int main2b(void){ // main2b
    Clock_Init80MHz(0);
    LaunchPad_Init();
    Lab5Grader(2);   // Scope
    DAC5_Init();     // your Lab 5 initialization
    Debug_Init();    // Lab 3 debugging
    while(1){
      for(uint32_t i=0; i<12; i++){ //0-11
        Testdata = Inputs[i];
        DAC5_Out(Testdata);
        Debug_Dump(Testdata);
        // use TExaSdisplay scope to measure DACout
        // place data in Table 5.3
        // touch and release S2 to continue
        while(LaunchPad_InS2()==0){}; // wait for S2 to be touched
        while(LaunchPad_InS2()!=0){}; // wait for S2 to be released
    }
  }
}
// use main3 to perform dynamic testing of DAC,
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a monotonic ramp with period 32ms,
int main3(void){ // main3
    Clock_Init80MHz(0);
    LaunchPad_Init();
    Lab5Grader(2);   // Scope
    DAC5_Init();     // your Lab 5 initialization
    Debug_Init();    // Lab 3 debugging
    while(1){
      for(uint32_t i=0; i<32; i++){ //0-31
        DAC5_Out(i);
        Debug_Dump(i);
        // scope to observe waveform
        Clock_Delay1ms(1);
    }
  }
}

// use main4 to debug the four input switches
int main4(void){ // main4
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Key_Init(); // your Lab 5 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 5, Fall 2023, Step 4. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Key_In(); // Your Lab5 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}


// use main5 to debug your system
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a sine wave with period/frequency depending on which key is pressed
int main(void){// main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Sound_Start(6499); // start one continuous wave
  while(1){
  }
}
// use main6 to debug/grade your final system
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a sine wave with period/frequency depending on which key is pressed
int main6(void){// main6
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Debug_Init();    // Lab 3 debugging
  uint32_t Pressed;
  uint32_t Last;
  while(1){
// if key goes from not pressed to pressed
//   -call Sound_Start with the appropriate period
//   -call Debug_Dump with period
// if key goes from pressed to not pressed
//   -call Sound_Stop
// I.e., if key has not changed DO NOT CALL start or stop
   uint32_t Pressed;
       Pressed= Key_In();

    if((Pressed != 0)&&(Last==0)){
        if(Pressed == 0x01){
            Sound_Start(7585);
            Debug_Dump(7585);
        } else if(Pressed == 0x02){
            Sound_Start(6020);
            Debug_Dump(6020);
        } else if(Pressed == 0x04){
            Sound_Start(5062);
            Debug_Dump(5062);
        } else if(Pressed == 0x08){
            Sound_Start(4509);
            Debug_Dump(4509);
        }else{Sound_Start(0);
    }
    if((Pressed == 0)&&(Last!=0))
        Sound_Stop();
    }
Last=Pressed;       //remember what was solved last


      //if switch is pressed then ewual to hex 0001, or 01,02, 04, 08. if thye match sound start, which the vaule of. then do debug dump and the same vaule

    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// To grade you must connect PB20 to your DACout
// Run main6 with Lab5Grader(3);   // Grader
// Observe Terminal window
// 5-bit 32-element sine wave
const uint8_t SinWave[32] = {
  16,19,22,24,27,28,30,31,31,31,30,
  28,27,24,22,19,16,13,10,8,5,4,
  2,1,1,1,2,4,5,8,10,13};

// Lab 5 low-level function implementations

// ARM SysTick period interrupts
// Input: interrupts every 12.5ns*period
//        priority is 0 (highest) to 3 (lowest)
void Sound_Init(uint32_t period, uint32_t priority){
SysTick->CTRL = 0x00;       //disable during init
SysTick->LOAD = period-1;   //set reload register
SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000))|priority<<30;
SysTick->VAL = 0;           //clear count
SysTick->CTRL = 0x07;       //Enable systick IRQ and Systick Timer

}
void Sound_Stop(void){
  // either set LOAD to 0 or clear bit 1 in CTRL
    // write this
SysTick->LOAD = 0;
}


void Sound_Start(uint32_t period){
    // write this
SysTick->LOAD = period - 1;

}

// Interrupt service routine
// Executed every 12.5ns*(period)
// Output at most one value to the DAC
void SysTick_Handler(void){static uint32_t i=0;
DAC5_Out (SinWave[i]);
i = (i + 1) & 0x1F;     //cycles through 0,1...,31,0,1,..   80M/(f*32)

}





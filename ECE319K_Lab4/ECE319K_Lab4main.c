/* ECE319K_Lab4main.c
 * Traffic light FSM
 * August 30, 2023
  */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two lines
const char EID1[] = "BC36587"; //  ;replace abc123 with your EID
const char EID2[] = "JXB223"; //  ;replace abc123 with your EID


#define WHITE (RED|GREEN|BLUE)


struct State {
    uint32_t Out;
    uint32_t Time;
    const struct State *Next[12];};
//may wish to change traffic out, forced the merge outputs, and then have to unmerge
typedef const struct State State_t;
#define GoSouth      &FSM[0]
#define WaitSouth    &FSM[1]
#define AllStop5_A   &FSM[2]
#define GoWalk       &FSM[3]
#define AllStop2_a   &FSM[4]
#define WaitWalk_a   &FSM[5]
#define AllStop2_b   &FSM[6]
#define WaitWalk_b   &FSM[7]
#define AllStop5_B   &FSM[8]
#define GoWest       &FSM[9]
#define WaitWest     &FSM[10]
#define AllStop5_C   &FSM[11]

State_t FSM[12] = {
            {0x04000101, 3000, {GoSouth, WaitSouth, WaitSouth, WaitSouth, GoSouth, WaitSouth, WaitSouth, WaitSouth}},
            {0x04000102, 500, {AllStop5_A, AllStop5_A, AllStop5_A, AllStop5_A, AllStop5_A, AllStop5_A, AllStop5_A, AllStop5_A}},
            {0x04000104, 500, {GoWalk, GoWest, GoWalk, GoWalk, GoSouth, GoWest, GoWalk, GoWalk}},
            {0x00400104, 3000, {GoWalk, AllStop2_a, GoWalk, AllStop2_a, AllStop2_a, AllStop2_a, AllStop2_a, AllStop2_a}},
            {0x04000104, 200, {WaitWalk_a, WaitWalk_a, WaitWalk_a, WaitWalk_a, WaitWalk_a, WaitWalk_a, WaitWalk_a, WaitWalk_a}},
            {0x08000104, 200, {AllStop2_b, AllStop2_b, AllStop2_b, AllStop2_b, AllStop2_b, AllStop2_b, AllStop2_b, AllStop2_b}},
            {0x04000104, 200, {WaitWalk_b, WaitWalk_b, WaitWalk_b, WaitWalk_b, WaitWalk_b, WaitWalk_b, WaitWalk_b, WaitWalk_b}},
            {0x08000104, 200, {AllStop5_B, AllStop5_B, AllStop5_B, AllStop5_B, AllStop5_B, AllStop5_B, AllStop5_B, AllStop5_B}},
            {0x04000104, 500, {GoWest, GoWest, GoWalk, GoWest, GoSouth, GoWest, GoSouth, GoWest}},
            {0x04000044, 3000, {GoWest, GoWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest}},
            {0x04000084, 500, {AllStop5_C, AllStop5_C, AllStop5_C, AllStop5_C, AllStop5_C, AllStop5_C, AllStop5_C, AllStop5_C}},
            {0x04000104, 500, {GoSouth, GoWest, GoWalk, GoWalk, GoSouth, GoSouth, GoSouth, GoSouth}}
};
State_t *Pt;

// initialize all 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ 
IOMUX->SECCFG.PINCM[PB15INDEX]          //THESE ARE SWITCHES, just need to initalize them
     =0X00040081; //INPUT PB15

IOMUX->SECCFG.PINCM[PB16INDEX]
     =0X00040081; //INPUT PB16

IOMUX->SECCFG.PINCM[PB17INDEX]
     =0X00040081; //INPUT PB17

//////////////////////////////////////////////////////////////////////////////////////////////

IOMUX->SECCFG.PINCM[PB0INDEX]
     =0X00000081; //OUTPUT PB0
GPIOB->DOE31_0 |= 0X01; //ENABLE OUTPUT PB0 GREEN LIGHT SOUTH, change all to or equals bc not friendly

IOMUX->SECCFG.PINCM[PB1INDEX]
     =0X00000081; //OUTPUT PB1
GPIOB->DOE31_0 |= 0X02; //ENABLE OUTPUT PB1 YELLOW LIGHT SOUTH, change all to or equals bc not friendly

IOMUX->SECCFG.PINCM[PB2INDEX]
     =0X00000081; //OUTPUT PB2
GPIOB->DOE31_0 |= 0X04; //ENABLE OUTPUT PB2 RED LIGHT SOUTH, change all to or equals bc not friendly

//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////

IOMUX->SECCFG.PINCM[PB6INDEX]
     =0X00000081; //OUTPUT PB6
GPIOB->DOE31_0 |= 0X20; //ENABLE OUTPUT PB6 GREEN LIGHT WEST, change all to or equals bc not friendly

IOMUX->SECCFG.PINCM[PB7INDEX]
     =0X00000081; //OUTPUT PB7
GPIOB->DOE31_0 |= 0X40; //ENABLE OUTPUT PB7 YELLOW LIGHT WEST, change all to or equals bc not friendly

IOMUX->SECCFG.PINCM[PB8INDEX]
     =0X00000081; //OUTPUT PB8
GPIOB->DOE31_0 |= 0X80; //ENABLE OUTPUT PB8 RED LIGHT WEST, change all to or equals bc not friendly

}
/* Activate LEDs
* Inputs: west is 3-bit value to three east/west LEDs
*         south is 3-bit value to three north/south LEDs
*         walk is 3-bit value to 3-color positive logic LED on PB22,PB26,PB27
* Output: none
* - west =1 sets west green
* - west =2 sets west yellow
* - west =4 sets west red
* - south =1 sets south green
* - south =2 sets south yellow
* - south =4 sets south red
* - walk=0 to turn off LED
* - walk bit 22 sets blue color
* - walk bit 26 sets red color
* - walk bit 27 sets green color
* Feel free to change this. But, if you change the way it works, change the test programs too
* Be friendly*/


void Traffic_Out(uint32_t west, uint32_t south, uint32_t walk){

    GPIOB->DOUT31_0 = (west << 6)  | (south << 0) | (walk);           //MAKE FRIENDLY, make a second one that just takes a single parameter and writes it to the output
}
void Traffic_Outsecond(uint32_t dataoutput){

    GPIOB->DOUT31_0 = dataoutput;           //MAKE FRIENDLY, make a second one that just takes a single parameter and writes it to the output
}


/* Read sensors
 * Input: none
 * Output: sensor values
 * - bit 0 is west car sensor
 * - bit 1 is south car sensor
 * - bit 2 is walk people sensor
* Feel free to change this. But, if you change the way it works, change the test programs too
 */
uint32_t Traffic_In(void){ uint32_t INPUT;

INPUT = (GPIOB->DIN31_0 &0x4000)>>15;

	// write this


    return INPUT;
}
// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
int main2beingtested(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab4Grader(0); // print assignment, no grading
  while(1){
  }
}
// use main2 to debug LED outputs
int main(void){ // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  UART_OutString("Lab 4, Fall 2023, Step 1. Debug LEDs\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    for(uint32_t i=1; i<8; i = i<<1){ //1,2,4
      Traffic_Out(i,0,0); // Your Lab 4 output
      Debug_Dump(i);
      Clock_Delay(40000000); // 0.5s
    }
    for(uint32_t i=1; i<8; i = i<<1){ //1,2,4
      Traffic_Out(0,i,0); // Your Lab 4 output
      Debug_Dump(i<<3);
      Clock_Delay(40000000); // 0.5s
    }
    Traffic_Out(0,0,RED); // Your Lab 4 output
    Debug_Dump(RED);
    Clock_Delay(40000000); // 0.5s
    Traffic_Out(0,0,WHITE); // Your Lab 4 output
    Debug_Dump(WHITE);
    Clock_Delay(40000000); // 0.5s
  }
}
// use main3 to debug the three input switches
int main3(void){ // main3
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Fall 2023, Step 2. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Traffic_In(); // Your Lab4 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// use main4 to debug using your dump
// proving your machine cycles through all states
int main4(void){// main4
uint32_t input;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Fall 2023, Step 3. Debug FSM cycle\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");

Pt = GoSouth;// initialize your FSM, ************not sure if this is what it wants?
SysTick_Init();// Initialize SysTick for software waits

  while(1){
   Traffic_Out(); // output using Traffic_Out
   Debug_Dump(); // call your Debug_Dump logging your state number and output
   SysTick_Wait10ms(Pt->Time); // wait
    input = 7; // hard code this so input always shows all switches
    Pt = Pt->Next[input]// next depends on state and input
  }
}
// use main5 to grade
int main5(void){// main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Pt = GoSouth;// initialize your FSM, SysTick
  SysTick_Init(); //initialize your FSM, SysTick

  Lab4Grader(1); // activate UART, grader and interrupts
  while(1){
  Traffic_Out();   // output using Traffic_Out
  SysTick_Wait10ms;   // wait
  Traffic_In();   // input using your Traffic_In
  Pt = Pt->Next[input]   // next depends on state and input
  }
}


// Lab6Main.c
// Runs on MSPM0G3507
// Test the functions in ST7735.c
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// Last Modified: 10/9/2023
// Ramesh Yerraballi modified 3/20/2017

#include "Lab6Grader.h"
#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "StringConversion.h"
#include "images.h"
// Modify these two with your EIDs
char EID1[]="JXB223";  // student's EID
char EID2[]="BC36587";  // student's EID

// use main0 to test udivby10
// does not need ST7735R connected
int main(void){ // main0
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Test_udivby10(); // defined in StringConversion.s
  while(1){
  }
}

// use main1 for testing
// does not need ST7735R connected
int main1(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
// Lab6Grader(1) to test SPIOutCommand, no grading
// Lab6Grader(2) to test SPIOutData, no grading
// Lab6Grader(3) to test GradeDec2String, no grading
// Lab6Grader(4) to test all three with grading
  Lab6Grader(2);
  while(1){
  }
}

#define SIZE 12
uint32_t const TestData[SIZE] ={
  0,7,99,100,654,999,1000,5009,9999,10000,20806,65535
};

char test[20];
// using main2 for demonstration
// needs ST7735R connected
int main2(void){
  uint32_t i;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_OutString("Lab 6 Fall 2023\n\xADHola!\nBienvenida al EE319K");
  while(LaunchPad_InS2()==0x00040000){}; // wait for release
  while(LaunchPad_InS2()==0){};          // wait for touch
  ST7735_FillScreen(0xFFFF);   // set screen to white
  ST7735_DrawBitmap(44, 159, Logo, 40, 160);
  while(LaunchPad_InS2()==0x00040000){}; // wait for release
  while(LaunchPad_InS2()==0){};          // wait for touch
  ST7735_FillScreen(0);       // set screen to black
  for(i=0; i<SIZE; i++){
    ST7735_SetCursor(0,i);
    Dec2String(TestData[i],test); // your Lab 6
    ST7735_OutString(test);
    ST7735_SetCursor(10,i);
    printf("%u",TestData[i]);  // stdio of similar function
  }
  while(LaunchPad_InS2()==0x00040000){}; // wait for release
  while(LaunchPad_InS2()==0){};          // wait for touch
  ST7735_FillScreen(0);       // set screen to black
  for(i=0;i<9;i++){
    ST7735_SetCursor(0,i);
    printf("%u",TestData[i]);  // stdio of similar function
    ST7735_SetCursor(8,i);
    printf("d=%1u.%.3u cm",TestData[i]/1000,TestData[i]%1000);  // fixed point output
  }
  while(1){
  }
}


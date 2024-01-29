// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "DAC5.h"
#include "../inc/Timer.h"

uint32_t SoundIndex;
uint8_t *SoundPt;
uint32_t SoundSize;
uint32_t sound_Count = 0;
void SysTick_IntArm(uint32_t period, uint32_t priority){
//SysTick->CTRL = 0x00;// write this, page 208 in textbook
SysTick->LOAD = period -1;  //set reload register
//priority 0 highest
//SCB ->SHP[1] = (SCB->SHP[1]&(0xC0000000))|priority<<30;
//SysTick->VAL = 0;
SysTick->CTRL = 0x07;       //enable systick irq and systick timer
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){
// write this
    SysTick_IntArm(1,0);
    DAC5_Init();
    SoundIndex = 0;
}
void SysTick_Handler(void){static uint32_t i=0; // called at 11 kHz
  // output one value to DAC if a sound is active
DAC5_Out(SoundPt[SoundIndex]>>3);
SoundIndex++;
if(SoundIndex >= SoundSize){
    SysTick->LOAD = 0;
    }
}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count){
// write this
    //SoundPt = pt;
    //SoundSize = count;

    //SoundIndex = 0;
    //SysTick->LOAD = 1; // Reload the SysTick to start the sound
}
void Sound_Shoot(void){
// write this
    SoundIndex = 0;
    SoundPt = shoot;
    SoundSize = 4080;
    SysTick->LOAD = 80000000/11025-1;   //11kHz
}
void Sound_Killed(void){
// write this
    SoundIndex = 0;
    SoundPt = invaderkilled;
    SoundSize = 3377;
    SysTick->LOAD = 80000000/11025-1;   //11kHz
}
void Sound_Explosion(void){
// write this
    SoundIndex = 0;
    SoundPt = explosion;
    SoundSize = 2000;
    SysTick->LOAD = 80000000/11025-1;   //11kHz
}

void Sound_Fastinvader1(void){
    SoundIndex = 0;
    SoundPt = fastinvader1;
    SoundSize = 982;
    SysTick->LOAD = 80000000/11025-1;   //11kHz
}
void Sound_Fastinvader2(void){
    SoundIndex = 0;
    SoundPt = fastinvader2;
    SoundSize = 1042;
    SysTick->LOAD = 80000000/11025-1;   //11kHz
}
void Sound_Fastinvader3(void){
    SoundIndex = 0;
    SoundPt = fastinvader3;
    SoundSize = 1054;
    SysTick->LOAD = 80000000/11025-1;   //11kHz
}
void Sound_Fastinvader4(void){
    SoundIndex = 0;
    SoundPt = fastinvader4;
    SoundSize = 1098;
    SysTick->LOAD = 80000000/11025-1;   //11kHz
}
void Sound_Highpitch(void){
    SoundIndex = 0;
    SoundPt = highpitch;
    SoundSize = 1802;
    SysTick->LOAD = 80000000/11025-1;   //11kHz
}



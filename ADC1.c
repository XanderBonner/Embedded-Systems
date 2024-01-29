/*ADC1.c
 *  Date: November 15th, 2023
 *  Author: Bryan Castro & Joss Bonner
 *  Slide Pot Module
 */
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"

void ADCinit(void){
// write code to initialize ADC1 channel 5, PB18
// Your measurement will be connected to PB18
// 12-bit mode, 0 to 3.3V, right justified
// software trigger, no averaging
    ADC1 -> ULLMEM.GPRCM.RSTCTL = 0xB1000003;       //RESET
        ADC1 -> ULLMEM.GPRCM.PWREN = 0x26000001;        //ACTIVATE
        Clock_Delay(24);                                //wait
        ADC1 -> ULLMEM.GPRCM.CLKCFG = 0xA900000;         //ULPCLK
        ADC1 -> ULLMEM.CLKFREQ = 7;                     //40-48 MHZ
        ADC1 -> ULLMEM.CTL0 = 0x03010000;               //DIVIDE BY 8
        ADC1 -> ULLMEM.CTL1 = 0x00000000;               //MODE
        ADC1 -> ULLMEM.CTL2 = 0x00000000;               //MEMRES
        ADC1 -> ULLMEM.MEMCTL[0] = 5;             //CHANNEL, FOR SURE NEED TO FIX
        ADC1 -> ULLMEM.SCOMP0 = 0;                      //8 SAMPLE CLOCKS
        ADC1 -> ULLMEM.CPU_INT.IMASK = 0;            //NO INTERRUPT
}
uint32_t ADCin(void){
  // write code to sample ADC1 channel 5, PB18 once
  // return digital result (0 to 4095)
    uint32_t volatile delay;
    ADC1 -> ULLMEM.CTL0 |= 0x00000001;          //ENABLE CONVERSION
    ADC1 -> ULLMEM.CTL1 |= 0x00000100;          //START ADC
    delay = ADC1->ULLMEM.STATUS;                  //TIME TO START
    while((ADC1->ULLMEM.STATUS&0x01)==0x01){};  //WAIT FOR COMPLETION
    return ADC1 ->ULLMEM.MEMRES[0];
}

// your function to convert ADC sample to distance (0.001cm)
// use main2 to calibrate the system fill in 5 points in Calibration.xls
//    determine constants k1 k2 to fit Position=(k1*Data + k2)>>12
uint32_t Convert(uint32_t input){
    return( (1879*input - 190)>>12 ); // replace this with a linear function
}

void OutFix(uint32_t n){
// resolution is 0.001cm
// n is integer 0 to 2000
// output to ST7735 0.000cm to 2.000cm
    uint32_t m;
    m = n % 1000;
    n = n /1000;


    printf(""
            " The d is =%d.%d ", n,m);
}

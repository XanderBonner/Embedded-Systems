// Lab7Main.c
// Runs on MSPM0G3507
// Lab 7 solution
// Bryan Castro
// Last Modified: 10/19/2023

#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}
#define ADCVREF_VDDA 0x000
#define ADCVREF_INT  0x200



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

    // write code to sample ADC1 channel 5, PB18 once
        uint32_t volatile delay;
        ADC1 -> ULLMEM.CTL0 |= 0x00000001;          //ENABLE CONVERSION
        ADC1 -> ULLMEM.CTL1 |= 0x00000100;          //START ADC
        delay = ADC1->ULLMEM.STATUS;                  //TIME TO START
        while((ADC1->ULLMEM.STATUS&0x01)==0x01){};  //WAIT FOR COMPLETION
        return ADC1 ->ULLMEM.MEMRES[0];

      // return digital result (0 to 4095)
              //FIXME


}

// your function to convert ADC sample to distance (0.001cm)
// use main2 to calibrate the system fill in 5 points in Calibration.xls
//    determine constants k1 k2 to fit Position=(k1*Data + k2)>>12
uint32_t Convert(uint32_t input){
  return( (1879*input - 190)>>12 ); // replace this with a linear function
}
float FloatConvert(uint32_t input){
  return 0.00048828125*input -0.0001812345;
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
void FloatOutFix(float x){
// resolution cm
// c is integer 0 to 2.000
// output to ST7735 0.000cm to 2.000cm
x = x /1000;
    printf("d=%f cm   ",x);  // floating point output
}

uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
float FloatPosition;  // 32-bit floating-point cm
uint32_t Flag;        // semaphore
uint32_t startTime,stopTime;
uint32_t Offset,ADCtime,Converttime,FloatConverttime,OutFixtime,FloatOutFixtime; // in bus cycles
uint32_t Time;
// use main1 if you do not have a voltmeter
// use main1 to test slidepot interface
// connect slidepot pin 2 to PB20, ADC1
// Open TExaSdisplay and see slidepot pin2 go from 0 to 3.3V
int main5beingtested(void){ // main1
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  TExaS_Init(ADC0,6,0); //PB20 = ADC0 channel 6, slidepot
  __enable_irq();
  while(1){
  }
}
// if you have a voltmeter/scope,
// use the voltmeter/scope see slidepot pin2 go from 0 to 3.3V

// use main2 to test ADCinit and ADCin functions
// connect slidepot pin 2 to PB18 ADC1
// Data should go from 0 to 4095
// ADCtime is the time to execute ADCin in bus cycles
// use main2 to calibrate the system fill in 5 points in Calibration.xls
//    determine constants k1 k2 to fit Position=(k1*Data)>>12 + k2
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  SysTick->LOAD = 0xFFFFFF;    // max
  SysTick->VAL = 0;            // any write to current clears it
  SysTick->CTRL = 0x00000005;  // enable SysTick with core clock
  startTime = SysTick->VAL;
  stopTime = SysTick->VAL;
  Offset = (startTime-stopTime)&0x0FFFFFF; // in bus cycles
  ADCinit(); //PB18 = ADC0 channel 5, slidepot
  while(1){
    startTime = SysTick->VAL;
    Data = ADCin();  // sample 12-bit ADC0 channel 5, slidepot
    stopTime = SysTick->VAL;
    ADCtime = ((startTime-stopTime)&0x0FFFFFF)-Offset; // in bus cycles
  }
}

// use main3 to test Convert functions
// connect slidepot pin 2 to PB18
// Data should go from 0 to 4095
// Position should go from 0 to 2000
// Use main3 to take another 5 measurements to determine accuracy
// truth is you eyes and the cursor on your slide pot
// measurement is the Position variable
// Converttime is the time to execute Convert in bus cycles
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  SysTick->LOAD = 0xFFFFFF;    // max
  SysTick->VAL = 0;            // any write to current clears it
  SysTick->CTRL = 0x00000005;  // enable SysTick with core clock
  startTime = SysTick->VAL;
  stopTime = SysTick->VAL;
  Offset = (startTime-stopTime)&0x0FFFFFF; // 25nsec
  ADCinit(); //PB18 = ADC0 channel 5, slidepot
  while(1){
    Data = ADCin();  // sample 12-bit ADC0 channel 5, slidepot
    startTime = SysTick->VAL;
    Position = Convert(Data);
    stopTime = SysTick->VAL;
    Converttime = ((startTime-stopTime)&0x0FFFFFF)-Offset; // in bus cycles
    startTime = SysTick->VAL;
    FloatPosition = FloatConvert(Data);
    stopTime = SysTick->VAL;
    FloatConverttime = ((startTime-stopTime)&0x0FFFFFF)-Offset; // in bus cycles
  }
}


// use main4 to test OutFix functions
// connect slidepot pin 2 to PB18
// Data should go from 0 to 4095
// Position should go from 0 to 2000
// LCD should show 0.000cm to 2.000 cm
// OutFixtime is the time to execute OutFix in bus cycles
int main4(void){ // main4
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  //note: if you colors are weird, see different options for
  // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  SysTick->LOAD = 0xFFFFFF;    // max
  SysTick->VAL = 0;            // any write to current clears it
  SysTick->CTRL = 0x00000005;  // enable SysTick with core clock
  ADCinit(); //PB18 = ADC0 channel 5, slidepot
  while(1){
    ST7735_SetCursor(0,0); // top left
    Data = ADCin();  // sample 12-bit ADC0 channel 5, slidepot
    Position = Convert(Data);
    FloatPosition = FloatConvert(Data);
    startTime = SysTick->VAL;
    OutFix(Position);
    stopTime = SysTick->VAL;
    OutFixtime = ((startTime-stopTime)&0x0FFFFFF)-Offset; // in bus cycles
    ST7735_SetCursor(0,1); // second row
    startTime = SysTick->VAL;
    FloatOutFix(FloatPosition);
    stopTime = SysTick->VAL;
    FloatOutFixtime = ((startTime-stopTime)&0x0FFFFFF)-Offset; // in bus cycles
    Clock_Delay1ms(100);
  }
}
// sampling frequency is 30 Hz
void TIMG12_IRQHandler(void){
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    Time++;

    Data = ADCin();
    Flag = 1;
 // sample 12-bit ADC0 channel 5, slidepot
 // store data into mailbox
 // set the semaphore
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}
// use main5 for final system
// use scope or logic analyzer to verify real time samples
// option 1) remove call to TExaS_Init and use a real scope on PB27
// option 2) use TExaS logic analyzer
int main(void){ // main5
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ADCinit(); //PB18 = ADC1 channel 5, slidepot
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
  ST7735_PlotClear(0,2000);
   TimerG12_IntArm(80000000/30,3); // initialize interrupts on TimerG12 at 30 Hz
  Time = 0;
  __enable_irq();
  while(1){

      while(Flag == 0){}
         Flag = 0;
      //   if(Flag){




     // wait for semaphore

    // clear the semaphore
    GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
    // get data from mailbox
    uint32_t main5D = Data;
    ST7735_SetCursor(0,0); // move cursor to top left
    // convert Data to Position
   main5D = Convert(main5D);//main5data
   OutFix(main5D);// display distance in top row OutFix
    if((Time%15)==0){
      ST7735_PlotPoint(Position);
      ST7735_PlotNextErase(); // data plotted at about 2 Hz
    }
  }
}

// use main6 to study Central Limit Theorem
uint32_t Histogram[64]; // probability mass function
uint32_t Center,SAC;
#define SCALE 2000
int main6(void){ // main6
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ADCinit(); //PB18 = ADC1 channel 5, slidepot
  SAC = 1;
  while(1){int i; uint32_t d,sum;
    sum = 0;
    for(int j=0; j<100; j++){
      sum += ADCin();  // sample 12-bit ADC0 channel 5, slidepot
    }
    Center = sum/100; // averaging
    for(i=0; i<64; i++) Histogram[i] = 0; // clear
    for(i=0; i<200; i++){
      sum = 0;
      for(int j=0; j<SAC; j++){
        sum += ADCin();  // sample 12-bit ADC0 channel 5, slidepot
      }
      Data = sum/SAC; // averaging
      if(Data<Center-32){
         Histogram[0]++;
      }else if(Data>=Center+32){
         Histogram[63]++;
      }else{
        d = Data-Center+32;
        Histogram[d]++;
      }
    }
    ST7735_PlotClear(0,100);
    ST7735_SetCursor(0,0);
    printf("SAC=%3u,Center=%4u",SAC,Center);
  //  ST7735_OutString("SAC=");
  //  ST7735_OutUDec(SAC);
  //  ST7735_OutString(",Center=");
  //  ST7735_OutUDec(Center);
  //  ST7735_OutString("  ");
    for(i=0; i<63; i++){
      if(Histogram[i]>99) Histogram[i]=99;
      ST7735_PlotBar(Histogram[i]);
      ST7735_PlotNext();
      ST7735_PlotBar(Histogram[i]);
      ST7735_PlotNext();
    }
    while(LaunchPad_InS2()==0x00040000){}; // wait for release
    while(LaunchPad_InS2()==0){};          // wait for touch
    SAC = SAC<<1;
    if(SAC >= 256) SAC = 1;
  }
}

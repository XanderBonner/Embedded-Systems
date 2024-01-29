/* UART1.c
 * Your name
 * Data:
 * PA8 UART1 Tx to other microcontroller PA22 UART2 Rx
 */

#include <ti/devices/msp/msp.h>
#include "UART1.h"
#include "../inc/Clock.h"
#define PA8INDEX  18 // UART1_TX  SPI0_CS0  UART0_RTS TIMA0_C0  TIMA1_C0N

// power Domain PD0
// for 80MHz bus clock, UART clock is ULPCLK 40MHz
// initialize UART1 for 2000 baud rate
// blind, no synchronization on transmit
void UART1_Init(void){
    // RSTCLR to GPIOA and UART0 peripherals
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset gpio port
//  GPIOA->GPRCM.RSTCTL = 0xB1000003; // called previously
  UART1->GPRCM.RSTCTL = 0xB1000003;                             //xander note: good
    // Enable power to GPIOA and UART0 peripherals
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
//  GPIOA->GPRCM.PWREN = 0x26000001; // called previously
  UART1->GPRCM.PWREN = 0x26000001;                              //activate uart, xnote: good
  Clock_Delay(24); // time for uart to power up
  // configure PA11 PA10 as alternate UART0 function
  IOMUX->SECCFG.PINCM[PA8INDEX]  = 0x00000082;                  //FIXME, hex number needs to be changed
  //bit 7  PC connected
  //bits 5-0=2 for UART0_Tx
  //bit 18 INENA input enable
  //bit 7  PC connected
  //bits 5-0=2 for UART0_Rx
  UART1->CLKSEL = 0x08; // bus clock
  UART1->CLKDIV = 0x00; // no divide
  UART1->CTL0 &= ~0x01; // disable UART1                        //FIXME, idk if we need to change hex bc we switch to UART1
  UART1->CTL0 = 0x00020018;
   // bit  17    FEN=1    enable FIFO
   // bits 16-15 HSE=00   16x oversampling
   // bit  14    CTSEN=0  no CTS hardware
   // bit  13    RTSEN=0  no RTS hardware
   // bit  12    RTS=0    not RTS
   // bits 10-8  MODE=000 normal
   // bits 6-4   TXE=001  enable TxD
   // bit  3     RXE=1    enable TxD
   // bit  2     LBE=0    no loop back
   // bit  0     ENABLE   0 is disable, 1 to enable
  /* old
  // 20000000/16 = 1,250,000 Hz
 // Baud = 115200
  //   1,250,000/115200 = 10.850694
  //   divider = 10+54/64 = 10.84375
  UART0->IBRD = 10;
  UART0->FBRD = 54; // baud =1,250,000/10.84375 = 115,274
  */
  if(Clock_Freq() == 40000000){
      // 20000000/16 = 1,250,000 Hz
     // Baud = 2000
      //   1,250,000/2000 = 625
      //   divider = 625+0/64 = 625
    UART1->IBRD = 625;
    UART1->FBRD = 0; // baud =1,250,000/10.84375 = 115,274
  }else if (Clock_Freq() == 32000000){
    // 32000000/16 = 2,000,000
     // Baud = 2000
      //   2,000,000/2000 = 1000
      //   divider = 1000+0/64 = 1000
      //2,000,000/2000 = 1000
      //divder = 1000+23/64=15.98
    UART1->IBRD = 1000;
    UART1->FBRD = 0;
  }else if (Clock_Freq() == 80000000){
     // 40000000/16 = 2,500,000 Hz
     // Baud = 115200
      //    2,500,000/115200 = 21.701388
      //   divider = 21+45/64 = 21.703125
      //2,500,000/2000= 1250
      //divider = 1250+45/64 = 20.234
    UART1->IBRD = 1250;
    UART1->FBRD = 0; // baud =2,500,000/21.703125 = 115,191
  }else return;
  UART1->LCRH = 0x00000030;
   // bits 5-4 WLEN=11 8 bits
   // bit  3   STP2=0  1 stop
   // bit  2   EPS=0   parity select
   // bit  1   PEN=0   no parity
   // bit  0   BRK=0   no break
  UART1->CTL0 |= 0x01; // enable UART1
}



//------------UART1_OutChar------------
// Output 8-bit to serial port
// blind synchronization
// 10 bit frame, 2000 baud, 5ms per frame
// Input: data is an 8-bit ASCII character to be transferred
// Output: none
void UART1_OutChar(char data){

    UART1->TXDATA = data;
// simply output data to transmitter without waiting or checking status

}

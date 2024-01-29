/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
    //GPIOB->GPRCM.RSTCTL = 0xB1000003;
    //GPIOB->GPRCM.PWREN  = 0x26000001;
    //Clock_Delay(24);
    IOMUX->SECCFG.PINCM[PA28INDEX]
        = 0x00040081;
    IOMUX->SECCFG.PINCM[PA16INDEX]
        = 0x00040081;

}
// return current state of switches
uint32_t Switch_In(void){

    return (GPIOA->DIN31_0>>16 & 0x00001001);


}



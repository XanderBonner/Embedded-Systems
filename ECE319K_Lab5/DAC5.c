/* DAC5.c
 * Students put your names here
 * Modified: October 4, 2023
 * 5-bit binary-weighted DAC connected to PB4-PB0
 */
#include <ti/devices/msp/msp.h>
#define PB0INDEX  11 // UART0_TX  SPI1_CS2  TIMA1_C0  TIMA0_C2
#define PB1INDEX  12 // UART0_RX  SPI1_CS3  TIMA1_C1  TIMA0_C2N
#define PB2INDEX  14 // UART3_TX  UART2_CTS I2C1_SCL  TIMA0_C3  UART1_CTS TIMG6_C0  TIMA1_C0
#define PB3INDEX  15 // UART3_RX  UART2_RTS I2C1_SDA  TIMA0_C3N UART1_RTS TIMG6_C1  TIMA1_C1
#define PB4INDEX  16 // UART1_TX  UART3_CTS TIMA1_C0  TIMA0_C2  TIMA1_C0N

// **************DAC5_Init*********************
// Initialize 5-bit DAC, called once
// Input: none
// Output: none
void DAC5_Init(void){
// Assumes LaunchPad_Init has been called
// I.e., PortB has already been reset and activated (do not reset PortB here again)
  // write this
    IOMUX->SECCFG.PINCM[PB0INDEX]
             =0X00000081; //OUTPUT PB0
        GPIOB->DOE31_0 |= 0X01; //ENABLE OUTPUT PB0, DAC bit 0

        IOMUX->SECCFG.PINCM[PB1INDEX]
             =0X00000081; //OUTPUT PB1
        GPIOB->DOE31_0 |= 0X02; //ENABLE OUTPUT PB1, DAC bit 0

        IOMUX->SECCFG.PINCM[PB2INDEX]
             =0X00000081; //OUTPUT PB2
        GPIOB->DOE31_0 |= 0X04; //ENABLE OUTPUT PB2, DAC bit 0

        IOMUX->SECCFG.PINCM[PB3INDEX]
             =0X00000081; //OUTPUT PB3
        GPIOB->DOE31_0 |= 0X08; //ENABLE OUTPUT PB3, DAC bit 0

        IOMUX->SECCFG.PINCM[PB4INDEX]
             =0X00000081; //OUTPUT PB4
        GPIOB->DOE31_0 |= 0X10; //ENABLE OUTPUT PB4, DAC bit 0


}

// **************DAC5_Out*********************
// output to DAC5
// Input: 5-bit data, 0 to 31
// Input=n is converted to n*3.3V/31
// Output: none
// Note: this solution must be friendly
void DAC5_Out(uint32_t data){
    GPIOB->DOUT31_0 &= ~0x1F;
    GPIOB->DOUT31_0 |= data;
}

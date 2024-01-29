// BusyWait.s
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

// Runs on any Cortex M0
// Use SPI to send an 8-bit code to the LCD.

// As part of Lab 6, students need to implement these two functions
// This driver assumes two low-level LCD functions
// this file should have been in the inc folder so it automatically will be applied to labs 7 8 and 9

      .global   SPIOutCommand
      .global   SPIOutData
      .text
      .align 2
// Used in ECE319K Labs 6,7,8,9. You write these two functions
// ***********SPIOutCommand*****************
// This is a helper function that sends an 8-bit command to the LCD.
// Inputs: R0 = 32-bit command (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI tx data register address
//         R3 = 32-bit GPIOA->DOUTCLR31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the clear register to clear will make it friendly
SPIOutCommand:
// --UUU-- Code to write a command to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
//3) Clear D/C (GPIO bit 13) to zero, be friendly (R3 has address of GPIOA->DOUTCLR31_0)
//4) Write the command to the SPI data register (R2 has address of SPI1->TXDATA)
//5) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)
PUSH {R4-R5,LR}

Step1Loop: LDR R4, [R1]		//load spi into r4
MOVS R5, #16			//MASK TO CLEAR BITS
ANDS R4, R5			//MASK SPI TO CHECK BIT 4
CMP R4, #0 			//CMP TO 16, BC THAT BIT 4 IN BINARY
BNE Step1Loop
LDR R4,[R3]			//move to step 3
MOVS R5, #1			//ADD 1 TO CLEAR GPIO BIT 13
LSLS R5, R5, #13
STR R5, [R3]		//STEP 3 FINISH
STR R0, [R2]		//STEP 4, WRITE COMMAND TO THE SPI DATA REGISTER
Step5Loop:
LDR R4, [R1]
MOVS R5, #16
ANDS R5, R4
CMP R5, #0
BNE Step5Loop
POP{R4-R5, PC}
// put solution here
    BX LR    //   return



// ***********SPIOutData*****************
// This is a helper function that sends an 8-bit data to the LCD.
// Inputs: R0 = 32-bit data (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI data register address
//         R3 = 32-bit GPIOA->DOUTSET31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the set register to clear will make it friendly
SPIOutData:
// --UUU-- Code to write data to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 1,
//2) If bit 1 is low, loop back to step 1 (wait for TNF bit to be high)
//3) set D/C (PA13) to one, be friendly (this is correct)
//4) Write the data to the SPI data register (R2 has address of SPI1->TXDATA)
// put solution here
PUSH {R4-R5,LR}
Step1Out: LDR R4, [R1]		//STEP 1 LOAD SPI STATUS REGISTER
MOVS R5, #2		//LOAD R5 WITH 2 SO WE CAN CLEAR
ANDS R4, R5		//CLEAR BITS EXCEPT 1
CMP R4, #0			//COMPARE R4 TO 1
BEQ Step1Out		//if 0 than loop back to 1
MOVS R5, #1		//THIS WILL SET TO 1, MOVE TO STEP 4
LSLS R5, R5, #13
STR R5, [R3]
STR R0, [R2]

POP{R4-R5, PC}
    BX LR    //   return

//****************************************************

    .end

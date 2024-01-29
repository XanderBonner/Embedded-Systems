//****************** ECE319K_Lab2_new.s ***************
// Your solution to Lab 2 in assembly code
// Author: Joss Bonner
// Last Modified: September 12th, 2023

   .equ GPIOB_RSTCTL,   0x400A2804
   .equ GPIOB_PWREN,    0x400A2800
   .equ GPIOB_DOE31_0,  0x400A32C0
   .equ GPIOB_DOUT31_0, 0x400A3280
   .equ GPIOB_DIN31_0,  0x400A3380
   .equ GPIOB_DOUTSET31_0, 0x400A3290
   .equ GPIOB_DOUTCLR31_0, 0x400A32A0
   .equ GPIOB_DOUTTGL31_0, 0x400A32B0
   .equ IOMUXPB0,       0x40428004+4*11
   .equ IOMUXPB1,       0x40428004+4*12
   .equ IOMUXPB2,       0x40428004+4*14
   .equ IOMUXPB3,       0x40428004+4*15
   .equ IOMUXPB16,      0x40428004+4*32
   .equ IOMUXPB17,      0x40428004+4*42
   .equ IOMUXPB18,      0x40428004+4*43

        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive
High:	.space 4
Low:	.space 4
   .equ OutBit,18  // PB18 output
   .equ InMask,8   // PB3 input

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "JXB223" // replace ZZZ123 with your EID here
        .align 2
  .equ dot,100
  .equ dash,(3*dot)
  .equ shortgap,(2*dot)  // not needed
  .equ interelement,dot
Morse:
  .long  dot,  dash,    0,    0, 0 // A
  .long  dash,  dot,  dot,  dot, 0 // B
  .long  dash,  dot, dash,  dot, 0 // C
  .long  dash,  dot,  dot,    0, 0 // D
  .long  dot,     0,    0,    0, 0 // E
  .long  dot,   dot, dash,  dot, 0 // F
  .long  dash, dash,  dot,    0, 0 // G
  .long  dot,   dot,  dot,  dot, 0 // H
  .long  dot,   dot,    0,    0, 0 // I
  .long  dot,  dash, dash, dash, 0 // J
  .long  dash,  dot, dash,    0, 0 // K
  .long  dot,  dash,  dot,  dot, 0 // L
  .long  dash, dash,    0,    0, 0 // M
  .long  dash,  dot,    0,    0, 0 // N
  .long  dash, dash, dash,    0, 0 // O
  .long  dot,  dash, dash,  dot, 0 // P
  .long  dash, dash,  dot, dash, 0 // Q
  .long  dot,  dash,  dot,    0, 0 // R
  .long  dot,   dot,  dot,    0, 0 // S
  .long  dash,    0,    0,    0, 0 // T
  .long  dot,   dot, dash,    0, 0 // U
  .long  dot,   dot,  dot, dash, 0 // V
  .long  dot,  dash, dash,    0, 0 // W
  .long  dash,  dot,  dot, dash, 0 // X
  .long  dash,  dot, dash, dash, 0 // Y
  .long  dash, dash,  dot,  dot, 0 // Z
  .align 2
  .global Lab2Grader
  .global Lab2
  .global Debug_Init // Lab3 programs
  .global Dump       // Lab3 programs

// Switch input: PB2 PB1 or PB0, depending on EID
// LED output:   PB18 PB17 or PB16, depending on EID
// logic analyzer pins PB18 PB17 PB16 PB2 PB1 PB0
// analog scope pin PB20
Lab2:
// Initially the main program will
//   set bus clock at 80 MHz,
//   reset and power enable both Port A and Port B
// Lab2Grader will
//   configure interrupts  on TIMERG0 for grader or TIMERG7 for TExaS
//   initialize ADC0 PB20 for scope,
//   initialize UART0 for grader or TExaS
	PUSH{R4, LR}
	BL Lab2Init		//branch to init to initalize your led Output

	MOVS R0, #1
	BL Lab2Grader

Delay:
   SUBS R0,R0,#2
dloop:
   SUBS R0,R0,#4 // C=1 if no overflow
   NOP           // C=0 on pass through 0
   BHS  dloop
   BX   LR



Delayms:
   PUSH {R4,LR}
   MOVS R4, #0			//CLEARS R4
   ADDS R4, R0
Continuedelay:
	LDR R0, =80000
	BL Delay
	SUBS R4, #1
	CMP R4, #0
	BNE Continuedelay
   POP  {R4,PC}
	BX LR

// make PB0 input, PB16 output
// PortB is already reset and powered
// Set IOMUX for your input and output
// Set GPIOB_DOE31_0 for your output (be friendly)
Lab2Init:
   LDR  R1,=0x00000081  			 // regular output pin
   LDR  R0,=IOMUXPB18
   STR  R1,[R0] 					//IOMUX->SECCFG.PINCM[PB18INDEX]  =  0x00000081;
   LDR  R1,=0x00040081
   LDR  R0,=IOMUXPB3
   STR  R1,[R0] 					//IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00040081; // input
   LDR  R0,=GPIOB_DOE31_0			//PUT IN SWITCH AND LED INTO THIS
   LDR  R2,[R0]
   LDR  R3,=(1<<18)					//SET BIT
   ORRS R2,R2,R3
   STR  R2,[R0] 					// PB16 output enable

loop:
Checkifpressed:
LDR R1, =GPIOB_DIN31_0
MOVS R3, #8
LDR R2, [R1]
ANDS R2, R2, R3
CMP R2, R3
BEQ Pressed
BNE Checkifpressed

Pressed:
MOVS R0, #1.		//LOGIC ANALYZER
BL Lab2Grader
LDR R2, =Morse
LDR R3, =0x41	//A IN INDEX
MOVS R4, #20
SUBS R3, R0, R3
MULS R3, R3, R4
ADDS R0, R2, R3
MOVS R4, #0
ADD R4, R0		//RELOAD R0 INTO R4

Checkwhichletter:
LDR R1, [R4]
LDR R2, =dash
CMP R1, R2
BEQ Dashdelay
LDR R3, =dot
CMP R1, R3
BEQ Dotdelay
CMP R1, #0

//BEQ JUMP
//JUMP:

B loop
POP{R4, PC}



LED_Off:  // PB16=0
  LDR  R1,=GPIOB_DOUTCLR31_0
  MOVS R0, 0X01
  LSLS R0, #18
  STR R0, [R1]
  BX LR
LED_On:  // PB16=1
  LDR  R1,=GPIOB_DOUTSET31_0
  MOVS R0, 0x01
  LSLS R0, #18			//shift 18 because bit
  STR R0, [R1]			//storing r0 into r1, puts 0 into pb18 which gets it turn off
  BX LR
Switch_Input:  // read PB0 input
  LDR  R1,=GPIOB_DIN31_0
  MOVS R3,#0x08
  LDR  R0,[R1]  // contents of GPIOB
  ANDS R0,R0,R3
  BX   LR

Dotdelay:
	ADDS R4, R4, #4
	BL LED_On
	LDR R0, =100
	BL Delayms
	BL LED_Off
	LDR R0, =100
	BL Delayms
	B Checkwhichletter
	BX LR

Dashdelay:
	ADDS R4, R4, #4
	BL LED_On			//turn on LED
	LDR R0, =300
	BL Delayms			//branch to delayms
	BL LED_Off
	LDR R0, =300
	BNE Checkifpressed
	BX LR

   .end

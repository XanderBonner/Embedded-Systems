//****************** ECE319K_Lab1.s ***************
// Your solution to Lab 1 in assembly code
// Author: Joss Bonner, EID: jxb223
// Last Modified: 8/31/23

        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "JXB223" // replace ZZZ123 with your EID here
        .align 2
        .global Phase
Phase:  .long 10 // 0 for info, 1-5 for debug, 10 for grade, step we are in the debugging phase

        .global Lab1
// Input: R0 points to the list
// Return: R0 as specified in Lab 1 assignment and terminal window
// According to AAPCS, you must save/restore R4-R7
// If your function calls another function, you must save/restore LR
Lab1: PUSH {R4-R7,LR}
MOV R4, R0		//CALLEE SAVE, LIST
LDR R0, [R0]    //list was loaded into r0
LDR R1, =EID	//loads my eid into r1 so we can have it be our first pointer
MOV R5, R1		//CALLEE SAVE, EID

runloop:
CMP R0, #0
BEQ notlisted


BL	StrCmp		//go to first round of string compare


CMP R0, #1
		BEQ	loadscore		//If loop found a match than we need to load the score from the eid
		BNE	Increment		//if not zero than increment by 8 and run again

Increment: MOV R0, R4		//RELOADS DESTROYED LIST
		   ADDS R4, #8      //This also needs to get incremented
		   ADDS R0, #8		//INCREMENT BY 8 TO GET TO NEXT EID
		   LDR R0, [R0]
		   MOV R1, R5		//RELOADS THE DESTROYED EID
		   B runloop


loadscore: MOV R0, R4
  		   ADDS R0, #4			//ADDS 4 ONTO R1, THIS GET US OUR SCORE
		   LDR R0, [R0]
		   B Finish


notlisted: MOVS R0, #0
		   SUBS R0, #1
		   B Finish

Finish:

//return

      POP  {R4-R7,PC} // return

StrCmp: ldrb R2, [R0]		//LOADS IN MY EID
		ldrb R3, [R1]		//LOAD FIRST CHAR OF STRING 1
		CMP R2, R3			// COMPARE STRINGS TO SEE IF THAT MATCH
		BNE no				//IF NOT EQAUL THAN WE BRANCH, STR0 != STR1
		CMP R2, #0			//IF STR0 REACHED NULL
		BEQ yes				//BOTH REACHED NULL, THERFORE STR0=STR1, AND WE ARE DONE
		ADDS R0, #1			//INCREMENT BOTH POINTERS TO NEXT CHAR
		ADDS R1, #1
		B StrCmp			//LOOP BACK TO STRING COMPARE
no:		MOVS R0, #0			//NO MATCH R0 = 0
		B done
yes:	MOVS R0, #1			//MATCH R0 =1
done:	BX LR				//function becomes terminated and lr means the return address will be sotred in the link

        .end




        //Copy and paste for lab 2

//****************** ECE319K_Lab2_new.s ***************
// Your solution to Lab 2 in assembly code
// Author: Joss Bonner
// Last Modified: September 12, 2023

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
	MOVS R0, #1		//LOGIC ANALYZER MODE
	BL Lab2Grader	//Logic analyzer, bus clock to 80 MHz
	//  BL   Debug_Init // your Lab3 (this will be completed as part of Lab 3)
	BL Lab2InIt		//branch to init to initalize your led Output
    //MOVS R7,#0   // index into highs
    // BL   SetDuty

loop:
BL LED_On
BL Delay
BL LED_Off
BL Delay
B loop
// step 1, wait for switch pressed
// step 2, call Lab2Grader
     MOVS R0,#0  // 0 for info, 1 debug with logic analyzer, 2 debug with scope, 10 for grade
     BL   Lab2Grader
// step 3, send letter in R0, returned from Lab2Grader

     B    loop


// delay
// input: R0 bus cycles
// output: none
Delay:
   SUBS R0,R0,#2 // overhead
dloop:
   SUBS R0,R0,#4 // C=1 if no overflow
   NOP           // C=0 on pass through 0
   BHS  dloop
   BX   LR


// delay ms
// input: R0 ms
// output: none
Delayms:
   PUSH {R4,LR}

   POP  {R4,PC}


// make PB0 input, PB16 output
// PortB is already reset and powered
// Set IOMUX for your input and output
// Set GPIOB_DOE31_0 for your output (be friendly)
Lab2Init:
  LDR  R1,=0x00000081   // regular output pin
   LDR  R0,=IOMUXPB18
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB18INDEX]  =  0x00000081;
   LDR  R1,=0x00040081

   LDR  R0,=IOMUXPB3
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00040081; // input
   LDR  R0,=GPIOB_DOE31_0			//PUT IN SWITCH AND LED INTO THIS
   LDR  R2,[R0]
   LDR  R3,=(1<<OutBit)
   ORRS R2,R2,R3
   STR  R2,[R0] // PB16 output enable
   BX   LR


LED_Off:  // PB16=0
  LDR  R1,=GPIOB_DOUTCLR31_0
  MOVS R0,#0x01
  LSLS R0,#OutBit
  STR  R0,[R1]
  BX   LR
LED_On:  // PB16=1
  LDR  R1,=GPIOB_DOUTSET31_0
  MOVS R0,#0x01
  LSLS R0,#OutBit
  STR  R0,[R1]
  BX   LR
//Switch_Input:  // read PB0 input
//  LDR  R1,=GPIOB_DIN31_0
//  MOVS R3,#InMask
//  LDR  R0,[R1]  // contents of GPIOB
//  ANDS R0,R0,R3
//  BX   LR




   .end

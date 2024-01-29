//****************** ECE319K_Lab2.s ***************
// Your solution to Lab 2 in assembly code
// Author: Your name
// Last Modified: Your date

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
High:   .space 4
Low:    .space 4
   .equ OutBit,18  // PB18 output
   .equ InMask,8   // PB3 input
        .text
        .thumb
        .align 2
        .global EID
EID:    .string "ZZZ1234" // replace ZZZ123 with your EID here
        .align 2
     .equ Freq,50  // 2 Hz
     .equ Cycles,(80000000/Freq/100)
     .equ Period,(Cycles*100)  // 500 ms
Highs0: .long Cycles*20, Cycles*40, Cycles*70, Cycles*80 // ms, to make 20, 40, 70, 80%
Highs1:  .long Cycles*10, Cycles*35, Cycles*50, Cycles*85   // Mode2=1
Highs2: .long Cycles*25, Cycles*45, Cycles*60, Cycles*85   // Mode2=2
Highs3: .long Cycles*15, Cycles*35, Cycles*55, Cycles*75   // Mode2=3
Highs4: .long Cycles*10, Cycles*30, Cycles*65, Cycles*70   // Mode2=4
Highs: .long Cycles*15, Cycles*35, Cycles*45, Cycles*75   // Mode2=5
Highs6: .long Cycles*10, Cycles*30, Cycles*40, Cycles*70   // Mode2=6
Highs7: .long Cycles*25, Cycles*35, Cycles*75, Cycles*85  // Mode2=7
        .global Lab2Grader
        .global Lab2
  .global Debug_Init
  .global Dump

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
     MOVS R0,#3  // 0 for info, 1 debug with logic analyzer, 2 debug with scope, 10 for grade
     BL   Lab2Grader
     BL   Debug_Init // your Lab3
     BL   Lab2Init // initialize input and output pins
     MOVS R7,#0   // index into highs
     BL   SetDuty
loop:
     BL   LED_On
     BL  Dump // record
     LDR  R0,=High
     LDR  R0,[R0]
     BL   Delay
     BL   LED_Off
     BL  Dump // record
     LDR  R0,=Low
     LDR  R0,[R0]
     BL   Delay
     BL   Switch_Input
     CMP  R0,#0
     BEQ  loop  // skip if not touched
wait4release:
     BL   Switch_Input
     CMP  R0,#0
     BNE  wait4release  // loop while touched
     ADDS R7,R7,#4
     MOVS R0,#0x0C
     ANDS R7,R7,R0  // 0,4,8,12
     BL   SetDuty
     B    loop
// delay
// input: R0 bus cycles
// output: none
Delay:
   SUBS R0,R0,#2
dloop:
   SUBS R0,R0,#4 // C=1 if no overflow
   NOP           // C=0 on pass through 0
   BHS  dloop
   BX   LR

// make PB0 input, PB16 output
// PortB is already reset and powered
// Set IOMUX for your input and output
// Set GPIOB_DOE31_0 for your output (be friendly)
Lab2Init:
   LDR  R1,=0x00000081   // regular output pin
   LDR  R0,=IOMUXPB16
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB16INDEX]  =  0x00000081;
   LDR  R0,=IOMUXPB17
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB17INDEX]  =  0x00000081;
   LDR  R0,=IOMUXPB18
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB18INDEX]  =  0x00000081;
   LDR  R1,=0x00040081
   LDR  R0,=IOMUXPB0
 //  STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB0INDEX] = 0x00040081; // input
   LDR  R0,=IOMUXPB1
//   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB1INDEX] = 0x00040081; // input
   LDR  R0,=IOMUXPB2
//   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB2INDEX] = 0x00040081; // input
   LDR  R0,=IOMUXPB3
   STR  R1,[R0] //IOMUX->SECCFG.PINCM[PB3INDEX] = 0x00040081; // input
   LDR  R0,=GPIOB_DOE31_0
   LDR  R2,[R0]
   LDR  R3,=(1<<OutBit)
   ORRS R2,R2,R3
   STR  R2,[R0] // PB16 output enable
   BX   LR

SetDuty:
   LDR  R3,=Highs
   LDR  R0,[R3,R7]
   LDR  R1,=High
   STR  R0,[R1]
   LDR  R2,=Period // 500 ms
   SUBS R0,R2,R0
   LDR  R1,=Low
   STR  R0,[R1]
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
Switch_Input:  // read PB0 input
  LDR  R1,=GPIOB_DIN31_0
  MOVS R3,#InMask
  LDR  R0,[R1]  // contents of GPIOB
  ANDS R0,R0,R3
  BX   LR
   .end

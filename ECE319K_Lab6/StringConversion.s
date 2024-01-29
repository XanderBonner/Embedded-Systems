// StringConversion.s
// Student names: Joss Bonner, Bryan Castro
// Last modification date: 10/17/23
// Runs on any Cortex M0
// ECE319K lab 6 number to string conversion
//
// You write udivby10 and Dec2String

    .global Dec2String
    .global Test_udivby10

    .text
    .align 2
// **test of udivby10**
// since udivby10 is not AAPCS compliant, we must test it in assembly
// feel free to edit this if you change how udivby10 works
Test_udivby10:
    PUSH {LR}

    MOVS R0,#123
    BL   udivby10
// put a breakpoint here
// R0 should equal 12 (0x0C)
// R1 should equal 3

    LDR R0,=12345
    BL   udivby10
// put a breakpoint here
// R0 should equal 1234 (0x4D2)
// R1 should equal 5

    MOVS R0,#0
    BL   udivby10
// put a breakpoint here
// R0 should equal 0
// R1 should equal 0
    POP {PC}

//****************************************************
// divisor=10
// Inputs: R0 is 16-bit dividend
// quotient*10 + remainder = dividend
// Output: R0 is 16-bit quotient=dividend/10
//         R1 is 16-bit remainder=dividend%10 (modulus)
// not AAPCS compliant because it returns two values
udivby10:
MOVS R3, #0			//THIS WILL BE OUR QUOTIENT, COUNTER
MOVS R4, #10		//THIS IS OUR DIVISOR, 10
Div_10_Loop:
CMP R0, R4
BLT Div_10_Done			//IF LESS THAN 10 THEN WE ARE DONE AND CAN STORE THE REMAINDER
SUBS R0, R0, R4			//SUBTRACT BY 10
ADDS R3, R3, #1			//ADD 1 TO QUOTIENT SO WE CAN KEEP TRACK
B Div_10_Loop

Div_10_Done:
MOV R1, R0				//MOV R0 INTO R1 BECAUSE THAT IS OUR REMAINDER
MOV R0, R3				//MOV THE QUOTIENT INTO R0
    BX LR    //   return



  
//-----------------------Dec2String-----------------------
// Convert a 16-bit number into unsigned decimal format
// String the string into the empty array add null-termination
// Input: R0 (call by value) 16-bit unsigned number
//        R1 pointer to empty array
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11
// you must implement local variables on the stack (binding, allocation, access and deallocation)
Dec2String:
// Step 1, binding(offset from SP)
//.equ var 2, #0 <- offset zero bc  SP start at var2
//.equ var 1, #4 <- offset 4 to get to next address
// Step 2, Allocation (decrement SP)
// SUB SP, #8 <- using 8 becuase we need to use 8 bytes to allocate 4 bytes for each var
// Step 3, Access(realtive to SP)
//LDR r0, [SP, #var]  <- this is the exact line of code, loads r0 with local variable 1
//Step 4 dealloctation(increment SP) (at the end of your code)
// ADD SP, #8   <- this is 8 because we used 8 bytes of data to store, so take away 8
//for num to ascii add 48
//increment and decrement sp


//LIST FROM VID
//1. Save registers for AAPCS
//2. Allocate local varibles
.equ var2, 0	//binding
.equ var1, 4	//binding
Allocate:
	PUSH {R7, LR}		//SAVE R7, AAPCS
	SUB SP, #8			//ALLOCATE 8
	MOV R7, SP			//FRAME POINTER
//3. Extract each digit, convert to ascii, push to stack

	PUSH {R0}
	LDR R0, [R7, #var1]	//ACCESS, using FRAME POINTER

	PUSH{R2}
	LDR R1, [R7, #var2]	//ACCESS, USING FRAME POINTER
//4. pop each digit, store in string
	POP {R3, R4}
//5. add null terminator to string

//6. Deallocate local vairbles
	ADD SP, #8			//DEALLOCATE
//7. restore registers for AAPCS
	POP {R7, PC}
// put solution here
    BX LR    //   return


//* * * * * * * * End of Dec2String * * * * * * * *
     .end

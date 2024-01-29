// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 solution
// Your name
// Last Modified: 11/6/2023
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "ADC1.h"
#include "DAC5.h"
#include "FIFO1.h"
#include "UART1.h"
#include "UART2.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz

void PLL_Init(void) { // set phase lock loop (PLL)
    Clock_Init40MHz(); // run this line for 40MHz

    // Clock_Init80MHz(0); // run this line for 80MHz
}

uint32_t M = 1;
uint32_t Random32(void) {
    M = 1664525*M + 1013904223;
    return M;
}

uint32_t Random(uint32_t n) {
    return (Random32() >> 16) % n;
}

void Delay100ms(uint32_t count);

/////// SPRITES ///////
typedef enum { dead, alive, dying } life_t;

struct enemy {
    int32_t x; // Range of x: 0 (left) to 127 (right)
    int32_t y; // Range of y: 0 (top) to 159 (bottom)
    double vx;
    double vy;
    uint32_t points;
    const unsigned short *image;
    const unsigned short *bimage;
    life_t life;
};

typedef struct enemy enemy_t;
enemy_t enemy[6];

// Player Bullet: x,y location; y velocity; images; dimensions; life

struct PlayerMissiles {
    int32_t x; // Range of x: 0 (left) to 127 (right)
    int32_t y; // Range of y: 0 (top) to 159 (bottom)
    int32_t vy; // Up is -1, down is +1
    const unsigned short *image;
    const unsigned short *bimage;
    int32_t h;
    int32_t w;
    life_t life;
};

typedef struct PlayerMissiles PlayerMissiles_t;
PlayerMissiles_t PlayerMissiles[50];

// Enemy Bullet: x,y location; y velocity; images; dimensions; life
struct enemyLasers {
    int16_t x; // Range of x: 0 (left) to 127 (right)
    int16_t y; // Range of y: 0 (top) to 159 (bottom)
    double vy; // Up is -1, down is +1
    const unsigned short *image;
    const unsigned short *bimage;
    int16_t h;
    int16_t w;
    life_t life;
    uint32_t time;
    uint32_t j;
    int Alldead;
    uint32_t Draw;
};

typedef struct enemyLasers enemyLasers_t;
enemyLasers_t enemyLasers[6];

// Player Ship: x,y location; images; dimensions; life
struct playerShip {
    int32_t x;
    int32_t y;
    const unsigned short *image;
    const unsigned short *bimage;
    int32_t h;
    int32_t w;
    life_t life;
};

typedef struct playerShip playerShip_t;
playerShip_t PlayerShip;

    void move();
    void draw();
    void SpaceInvadersInit();
    void ProcessInput();
    // void Delay100ms();
    uint32_t Data;
    uint32_t Flag;
    uint32_t Index = 0;
    uint32_t OldPosition;
    uint32_t Position;
    uint32_t Count = 0;
    uint32_t Pause;

// games engine runs at 30Hz

void TIMG12_IRQHandler(void) {
    uint32_t pos, msg;
    if ((TIMG12->CPU_INT.IIDX) == 1) { // this will acknowledge
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)

/////// game engine goes here///////

        Data = ADCin(); // Sample ADC for slide pot

        OldPosition = Position;

        Position = ((127 - 18) * Data) / 4095;

        ProcessInput(); // Sample buttons

        Count++;

        move(); // Move sprite

        Sound_Init();

        Flag = 1; // Signal semaphore
    }



    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void){

    return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
{Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
{Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
{Language_English,Language_Spanish,Language_Portuguese,Language_French}
};

// use main1 to observe special characters
int main1(void){ // main1
    char l;
    __disable_irq();
    PLL_Init(); // set bus speed
    LaunchPad_Init();
    ST7735_InitPrintf();
    ST7735_FillScreen(0x0000); // set screen to black
        for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
            for(Language_t myL=English; myL<= French; myL++){
                ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
                ST7735_OutChar(' ');
                ST7735_OutString((char *)Phrases[myPhrase][myL]);
                ST7735_OutChar(13);
            }
        }
    Clock_Delay1ms(3000);
    ST7735_FillScreen(0x0000); // set screen to black
    l = 128;
    while(1){
        Clock_Delay1ms(2000);
        for(int j=0; j < 3; j++){
            for(int i=0;i<16;i++){
                ST7735_SetCursor(7*j+0,i);
                ST7735_OutUDec(l);
                ST7735_OutChar(' ');
                ST7735_OutChar(' ');
                ST7735_SetCursor(7*j+4,i);
                ST7735_OutChar(l);
                l++;
            }
        }
    }
}
// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
 /*
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  */

  ST7735_FillScreen(0x0000); // set screen to black
  ST7735_SetCursor(4, 0);
  ST7735_OutString("SPACE INVADERS");
  ST7735_SetCursor(10, 1);
  ST7735_OutString("by");
  ST7735_SetCursor(2, 2);
  ST7735_OutString("BC & JB");
  ST7735_SetCursor(2, 5);
  ST7735_OutString("English ->");
  ST7735_SetCursor(2, 6);
  ST7735_OutString("Up Button");
  ST7735_SetCursor(2, 11);
  ST7735_OutString("Espanol ->");
  ST7735_SetCursor(2, 12);
  ST7735_OutString("Left Button");

  for(uint32_t t=9; t<150; t=t+1){
      ST7735_DrawBitmap(4,t,SmallEnemy10pointA,16,10);
      ST7735_DrawBitmap(24,t,SmallEnemy10pointA,16,10); //64 is middle since 128 wide
      ST7735_DrawBitmap(44,t,SmallEnemy10pointA,16,10);
      ST7735_DrawBitmap(64,t,SmallEnemy10pointA,16,10);
      ST7735_DrawBitmap(84,t,SmallEnemy10pointA,16,10);
      ST7735_DrawBitmap(104,t,SmallEnemy10pointA,16,10);
      //moving verticaly down from the top middle.
      SmallFont_OutVertical(t,104,6); //top left
      Clock_Delay1ms(33);
  }
  ST7735_DrawBitmap(4,150,SmallEnemy10pointBlank,16,10);
  ST7735_DrawBitmap(24,150,SmallEnemy10pointBlank,16,10);
  ST7735_DrawBitmap(44,150,SmallEnemy10pointBlank,16,10);
  ST7735_DrawBitmap(64,150,SmallEnemy10pointBlank,16,10);
  ST7735_DrawBitmap(84,150,SmallEnemy10pointBlank,16,10);
  ST7735_DrawBitmap(104,150,SmallEnemy10pointBlank,16,10);

/*
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);*/


  while(1){
  }
}
// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs
    uint32_t data1 = (Switch_In() & 0x00001000);



    uint32_t data4 = (Switch_In() & 0x00000001);

  }
}


// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
    __disable_irq();
    PLL_Init(); // set bus speed
    LaunchPad_Init();
    Switch_Init(); // initialize switches
    LED_Init(); // initialize LED
    Sound_Init(); // initialize sound
    TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
    __enable_irq();
    while(1){
        now = Switch_In(); // one of your buttons
        if((last == 0)&&(now == 1)){
            Sound_Shoot(); // call one of your sounds
        }
        if((last == 0)&&(now == 2)){
            Sound_Killed(); // call one of your sounds
        }
        if((last == 0)&&(now == 4)){
            Sound_Explosion(); // call one of your sounds
        }
        if((last == 0)&&(now == 8)){
            Sound_Fastinvader1(); // call one of your sounds
        }
last = now;
// modify this to test all your sounds
    }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN

uint16_t next = 0;
void firstup(void){
    ST7735_DrawBitmap(0, 0, marquee, 68, 128);
    ST7735_FillScreen(0x0000); // set screen to black
    ST7735_SetCursor(4, 0);
    ST7735_OutString("SPACE INVADERS");
    ST7735_SetCursor(9, 1);
    ST7735_OutString("by");
    ST7735_SetCursor(4, 2);
    ST7735_OutString("BC & JB");
    ST7735_SetCursor(2, 5);
    ST7735_OutString("English ->");
    ST7735_SetCursor(2, 6);
    ST7735_OutString("Top Button");
    ST7735_SetCursor(2, 11);
    ST7735_OutString("Espanol ->");
    ST7735_SetCursor(2, 12);
    ST7735_OutString("Boton de abajo");

    // next = 1 for english, 0 for spanish

    uint8_t state1 = 0;
    while(state1 == 0){

        uint32_t data0PA28 = (Switch_In() & 0x00001000);
        uint32_t data0PA16 = (Switch_In() & 0x00000001);

    if(data0PA28){ // pin 28
        next = 1;
        state1 = 1;
        }
    if(data0PA16){ // pin 16
        state1 = 1;
        //next = 0;
        }
    }
    ST7735_FillScreen(0x0000);
    if(next == 1){
        ST7735_SetCursor(4, 0);
        ST7735_OutString("SPACE INVADERS");
        ST7735_SetCursor(2, 6);
        ST7735_OutString("Hello");
        ST7735_SetCursor(2, 9);
        ST7735_OutString("press the bottom");
        ST7735_SetCursor(2, 10);
        ST7735_OutString("button to");
        ST7735_SetCursor(2, 11);
        ST7735_OutString("shoot bullets");
        }
    if(next == 0){
        ST7735_SetCursor(4, 0);
        ST7735_OutString("SPACE INVADERS");
        ST7735_SetCursor(2, 12);
        ST7735_OutString("\xADHola!");
        ST7735_SetCursor(2, 6);
        ST7735_OutString("presione el boton");
        ST7735_SetCursor(4, 7);
        ST7735_OutString("de abajo para");
        ST7735_SetCursor(3, 8);
        ST7735_OutString("disparar balas");
        }
    Delay100ms(20);
    ST7735_FillScreen(0x0000);
    }
    uint8_t lives = 1;
    static uint8_t score=0;
    uint32_t x1;
    uint32_t y1;
    uint32_t x2;
    uint32_t y2;
    uint32_t count1;

///////MOVE///////

void move(void) {

// enemy movement //
    for (int i = 0; i < 6; i++) {

     if (enemy[i].life == alive) {

     if (enemy[i].y >= 159) {
             lives--;

     if (lives ==0){
        Sound_Killed();
}
}
     count1 +=2;

     if(count1 == 50){
     enemy[i].y += (enemy[i].vy);
     count1 =0;
}
}
}
// Player missile movement //
    for (int i = 0; i < 50; i++) {

    if (PlayerMissiles[i].life == alive) {
       Sound_Shoot();
       x1 = PlayerMissiles[i].x + 4;
       y1 = PlayerMissiles[i].y - 6;
       if ((PlayerMissiles[i].y < 4) || (PlayerMissiles[i].y > 159)) {
       PlayerMissiles[i].life = dying;
}
       else {
             PlayerMissiles[i].y = PlayerMissiles[i].y + PlayerMissiles[i].vy;
}

       for(int j = 0; j < 30; j++){
          x2 = enemy[j].x + 12;
          y2 = enemy[j].y - 7;

          if(enemy[j].life == alive){

              if((((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2))) < 20){
                score++;
                enemy[j].image = SmallEnemy10pointBlank;
                Sound_Shoot(); // If sound is needed for enemy killed
}}}}}

        for (int i=0; i < 6; i++) {
        if (enemyLasers[i].life == alive) {
            x1 = enemyLasers[i].x;
            y1 = enemyLasers[i].y;
            if (enemyLasers[i].y > 159) {
                enemyLasers[i].life = dead;
}
            else {
                enemyLasers[i].y += enemyLasers[i].vy;
}}}}

//////MAIN 5//////

uint8_t game;
int flag;


int main(void){ // final main
    __disable_irq();
    PLL_Init(); // set bus speed
    LaunchPad_Init();
        //UART1_Init(); // just transmit, PA8, blind synchronization
        //UART2_Init(); // just receive, PA22, receiver timeout synchronization
    ST7735_InitPrintf();
        //note: if you colors are weird, see different options for
        // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
    ST7735_FillScreen(ST7735_BLACK);
    ADCinit(); //PB18 = ADC1 channel 5, slidepot //ADC1init();
    Switch_Init(); // initialize switches
    SpaceInvadersInit();
    LED_Init(); // initialize LED
    Sound_Init(); // initialize sound
    TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    TimerG12_IntArm((80000000/30), 3); // initialize interrupts on TimerG12 at 30 Hz

    // initialize all data structures

    __enable_irq();

    // wait for semaphore
    // flag=0; // clear semaphore
    // update ST7735R

    game = 1;

    while(1){
         // wait for semaphore
        flag=0; // clear semaphore

        firstup();
        while(game == 1){
                move();
                draw();
                for(int f = 0; f < 6 ; f++){
                if(enemy[f].y == (159 - PlayerShip.h)){
                   game = 0;
}
}
}
        ST7735_FillScreen(0x0000);
        if(next == 0){
        ST7735_SetCursor(2, 2);
        ST7735_OutString("GAME OVER");
        if(score == 0){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("SCORE - 0");
        }
        if(score > 0){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("SCORE - 1");
        }
        if(score > 9){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("SCORE - 2");
        }
        if(score > 15){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("SCORE - 3");
        }
        if(score > 25){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("SCORE - 4");
        }
        Delay100ms(100);
        }
        ST7735_FillScreen(0x0000);
        if(next == 1){
        ST7735_SetCursor(2, 2);
        ST7735_OutString("Juego Finalizado");
        if(score == 0){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("PUNTOS - 0");
        }
        if(score > 0){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("PUNTOS - 1");
        }
        if(score > 9){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("PUNTOS - 2");
        }
        if(score > 15){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("PUNTOS - 3");
        }
        if(score > 25){
        ST7735_SetCursor(4, 4);
        ST7735_OutString("PUNTOS - 4");
        }
        Delay100ms(100);
}
    uint32_t data1PA28 = (Switch_In() & 0x00001000);

    while((data1PA28) == 1){};
    game = 1;
    ST7735_FillScreen(0x0000);
// check for end of game
}
}

/// DRAW ///
void SpaceInvadersInit(void) {
    // Initialize player bullets
    for (int i = 0; i < 50; i++) {
        PlayerMissiles[i].life = dead;
        PlayerMissiles[i].image = Missile0;
        PlayerMissiles[i].bimage = Missile2;
        PlayerMissiles[i].h = 8;
        PlayerMissiles[i].w = 4;
}
// Initialize enemies

    for(int i=0; i<6 ; i++){
        enemy[i].x = 20*i+12;
        enemy[i].y = 10;
        enemy[i].vx = 0;
        enemy[i].vy = 1;
        enemy[i].points = 10;
        enemy[i].image = SmallEnemy10pointA;
        enemy[i].bimage = SmallEnemy10pointBlank;
        enemy[i].life = alive;}


// Initialize player ship
    PlayerShip.y = 18;
    PlayerShip.image = PlayerShip0;
    PlayerShip.bimage = PlayerShipblank;
    PlayerShip.life = alive;
}


    uint32_t Data;
    uint32_t Position;
    uint32_t OldPosition;

void draw(void) {
    //Playership
    int32_t distance;
    distance = OldPosition - Position;
    if ((distance > 2) || (distance < -2)) {
    ST7735_DrawBitmap(OldPosition, 159, SmallEnemy10pointBlank, 18, 8);
    ST7735_DrawBitmap(Position, 159, PlayerShip0, 18, 8);
    } else {
    ST7735_DrawBitmap(Position, 159, PlayerShip0, 18, 8);
    }
    //draw enemy and lasers
    for (int i = 0; i < 6; i++) {
        if (enemy[i].life == alive) {
            ST7735_DrawBitmap(enemy[i].x, enemy[i].y, enemy[i].image, 16, 10);

            }
        if (enemy[i].life == dying) {
            ST7735_DrawBitmap(enemy[i].x, enemy[i].y, enemy[i].bimage, 16, 10);
            enemy[i].life = dead;
            }
        if (enemyLasers[i].life == alive) {
            ST7735_DrawBitmap(enemyLasers[i].x, enemyLasers[i].y, enemyLasers[i].image,
            enemyLasers[i].w, enemyLasers[i].h);
            Sound_Shoot();
            }
        if (enemyLasers[i].life == dying) {
            ST7735_DrawBitmap(enemyLasers[i].x, enemyLasers[i].y, enemyLasers[i].bimage,
            enemyLasers[i].w, enemyLasers[i].h);
            enemyLasers[i].life = dead;
            }
    }
        //draw missiles
    for (int i = 0; i < 30; i++) {
        if (PlayerMissiles[i].life == alive) {
            ST7735_DrawBitmap(PlayerMissiles[i].x, PlayerMissiles[i].y, PlayerMissiles[i].image,
            PlayerMissiles[i].w, PlayerMissiles[i].h);
            }
        if (PlayerMissiles[i].life == dying) {
            ST7735_DrawBitmap(PlayerMissiles[i].x, PlayerMissiles[i].y, PlayerMissiles[i].bimage,
            PlayerMissiles[i].w, PlayerMissiles[i].h);
            PlayerMissiles[i].life = dead;
            }
    }
}



void ProcessInput(void) {
uint32_t Present;
uint32_t Present1;
uint32_t Finalenemy;
//uint32_t data10 = (Switch_In() & 0x00001000);
uint32_t data2PA16 = (Switch_In() & 0x00000001);
Present = (data2PA16);
    if ((Present == 1) && (Finalenemy == 0)) {
        Sound_Shoot();
        PlayerMissiles[0].life = alive;// add so others bullets don't stop
        if ((PlayerMissiles[0].vy = -1)){
        }
        PlayerMissiles[0].x = Position + 7;
        PlayerMissiles[0].y = 150;
    }

}



void Delay100ms(uint32_t count){uint32_t volatile time;

while(count>0){
        time = 727240; // 0.1sec at 80 MHz

while(time){
     time--;}
     count--;}
}


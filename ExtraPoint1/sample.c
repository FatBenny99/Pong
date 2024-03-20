/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "adc/adc.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

// SCHERMO 240 * 320 
/* 0,0-----------240
    |-------------|
    |-------------|
    |------------x|<-
		|-----------x-|
		|---------x---|
		|-------x-----|
		|xxxxxxxxxxxxxx
		320------------ */
		
//variabili globali della palla
volatile int x_ball;
volatile int y_ball;

//variabili globali del paddle
volatile int toprightx,toprighty;
volatile int bottomleftx,bottomlefty;

//score
volatile int record;
volatile int game_score;

//ball movement
volatile int direction;
volatile int points;
volatile int speed;

//buttons
volatile int key;


int main(void)

{
  char str[10];
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	//inizializzazione variabili globali
  points=0;
	speed=2;
	key=-1;
	record=100;
	
	TP_Init();
	//costruzione schermo
	LCD_Clear(Black);
	LCD_Bordi(5);
	LCD_Paddle(64,10,90,320-32);
	sprintf(str,"%d",points);
  GUI_Text(6,160,(uint8_t *) str,Green,Black);
	sprintf(str,"%d",record);
	GUI_Text(200,15,(uint8_t*) str,Green,Black);
	LCD_Ball(232,160);
	direction=DOWN_LEFT;
	
	//configurazione ADC
	ADC_init();													
	
	//configurazioni dello speaker
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	 init_timer(1,	0x000061A8);					   /*TIMER1 Inizializzazione per gestire i bottoni come se fosse un RIT    */
	 init_timer(2,0x8480);                   //TIMER2 per i rimbalzi 
	 enable_timer(1);
	 
	 LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	 LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
	//	__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

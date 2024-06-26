/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../RIT/rit.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



extern int x_ball;
extern int y_ball;
extern int direction;
extern int points;
extern int speed;


extern int record;
extern int game_score;

extern int toprightx,toprighty;
extern int bottomleftx,bottomlefty;

extern int key;

int timer2en=0;
int timer3en=0;



uint16_t SinTable[45] =                                       /*                      */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};



/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void 	reset_paddle()
{
int i;
	for(i=6;i<235;i++)
	{
	LCD_DrawLine(i,6,i,toprighty-2,Black);
	}

}



void ndown_nright(int ndown,int nright,int dir1) //Dir1 � la direzione se sbatte a destra, dir2 se sbatte sopra
{
		char punteggio[10];

								LCD_EraseBall(x_ball,y_ball);
									x_ball=x_ball+nright*speed;
									y_ball=y_ball+ndown*speed;
									if(x_ball+2 >=235 && y_ball+2<toprighty)  //se sbatte a destra
									{
										x_ball=232;
										LCD_Ball(x_ball,y_ball);
										direction=dir1;
										init_timer(2,0x848);
										reset_timer(2);
										enable_timer(2);
										disable_timer(0);
										timer2en=1;
										
									}
									else if(y_ball>=toprighty-2 && x_ball>bottomleftx && x_ball<toprightx)
									{
									  y_ball=toprighty-3;
									  LCD_Ball(x_ball,y_ball);
									  direction=((LPC_ADC->ADGDR>>4) & 0xFFF)%6;            //si decide la direzione random in base al valore del potenziometro
											if(points<100) points=points+5;
											else points+=10;
											//aggiorno il punteggio
											sprintf(punteggio,"%d",points);
										  GUI_Text(6,160,(uint8_t *) punteggio,Green,Black);
										  init_timer(2,0x424);                   
											reset_timer(2);  
										  enable_timer(2);       
											timer2en=1;
										disable_timer(0);
										sprintf(punteggio,"%d",record);
										GUI_Text(200,15,(uint8_t*) punteggio,Green,Black);

									}
									else if(y_ball>bottomlefty && (x_ball>toprightx+2 || x_ball<bottomleftx-2))
									{
										y_ball=bottomlefty+3;
										LCD_EraseBall(x_ball,y_ball);
										disable_timer(0);
										reset_paddle();

										if(points>record)
										{
										sprintf(punteggio,"%d",points);
										GUI_Text(200,15,(uint8_t*) punteggio,Green,Black);
										record=points;
										}
										
										points=0;
										sprintf(punteggio,"%d",points);
										GUI_Text(100,190,(uint8_t *) "You Lose",Green,Black);
										//GUI_Text(6,160,(uint8_t *) punteggio,Green,Black);
									}
									else LCD_Ball(x_ball,y_ball);
}



void ndown_nleft(int ndown,int nleft,int dir1)   //Dir1 quando sbatte al bordo sinistro
{		
		char punteggio[10];

									LCD_EraseBall(x_ball,y_ball);
									x_ball=x_ball-nleft*speed;
									y_ball=y_ball+ndown*speed;
										//check se si � arrivati al bordo sinistro MA non al paddle
									if(x_ball-2 <= 5 && y_ball<toprighty+2) 
									{
									x_ball=8;     //se si � arrivati al bordo devo impostare la direzione uguale al bordo
									LCD_Ball(x_ball,y_ball);
									direction=dir1;
								  reset_timer(2);									
									init_timer(2,0x848);
									enable_timer(2);
									disable_timer(0);
									timer2en=1;
											
									}
									else if(y_ball>=toprighty-2 && x_ball>bottomleftx && x_ball<toprightx) //se sta toccando il paddle 
									{
										y_ball=toprighty-3;
									  LCD_Ball(x_ball,y_ball);
										
									  direction=((LPC_ADC->ADGDR>>4) & 0xFFF)%6;            //si decide la direzione random
											if(points<100) points=points+5;
											else points+=10;
											//aggiorno il punteggio
											sprintf(punteggio,"%d",points);
										  GUI_Text(6,160,(uint8_t *) punteggio,Green,Black);
										  reset_timer(2);
											init_timer(2,0x424);
										  enable_timer(2);    
									    disable_timer(0);
											timer2en=1;
										// se la pallina durante i rimbalzi ha sovrascritto il record
										sprintf(punteggio,"%d",record);
										GUI_Text(200,15,(uint8_t*) punteggio,Green,Black);

									}
									else if(y_ball>bottomlefty && (x_ball>toprightx+2 || x_ball<bottomleftx-2))
									{
										y_ball=bottomlefty+3;
										LCD_EraseBall(x_ball,y_ball);
										disable_timer(0);
										reset_paddle();

										if(points>record)
										{
										sprintf(punteggio,"%d",points);
										GUI_Text(200,15,(uint8_t*) punteggio,Green,Black);
										record=points;
										}
										
										points=0;
										sprintf(punteggio,"%d",points);
										GUI_Text(100,190,(uint8_t *) "You Lose",Green,Black);

									}
									else LCD_Ball(x_ball,y_ball);


}

void nup_nright(int nup,int nright,int dir1,int dir2) //dir1 se sbatte a destra, dir2 se sbatte sopra
{
									LCD_EraseBall(x_ball,y_ball);
									x_ball=x_ball+nright*speed;
									y_ball=y_ball-nup*speed;
									if(x_ball+2 >=235 && y_ball-2>5)   //se sbatte a destra
									{
									 x_ball=232;
									 LCD_Ball(x_ball,y_ball);
										direction=dir1;
									reset_timer(2);
									init_timer(2,0x848);
									enable_timer(2);
									timer2en=1;
									disable_timer(0);

									
									}
									else if(x_ball+2<235 && y_ball-2<=5) //sbatte sopra
									{
									y_ball=8;
									LCD_Ball(x_ball,y_ball);
									direction=dir2;
									reset_timer(2);
									init_timer(2,0x848);
									enable_timer(2);
									disable_timer(0);
									timer2en=1;
									
									}
									else LCD_Ball(x_ball,y_ball);


}


void nup_nleft(int nup,int nright,int dir1, int dir2) //dir1 se sbatte a sinsitra, dir 2 se sbatte sopra
{
	
	
	

									LCD_EraseBall(x_ball,y_ball);
									x_ball=x_ball-nright*speed;
									y_ball=y_ball-nup*speed;			
									if(x_ball-2 <= 5 && y_ball-2 >5)  //se sbatte a sinistra ma non nel muro in alto
									{
									x_ball=8;
									LCD_Ball(x_ball,y_ball);
									direction=dir1;
									init_timer(2,0x848);
									reset_timer(2);
									enable_timer(2);	
									timer2en=1;
									disable_timer(0);
									}
									else if(x_ball-2>5 && y_ball-2<=5)  //se sbatte sopra
									{
									y_ball=8;
									LCD_Ball(x_ball,y_ball);
									direction=dir2;
									reset_timer(2);
									init_timer(2,0x848);
									enable_timer(2);
									disable_timer(0);
									timer2en=1;
									}
									else LCD_Ball(x_ball,y_ball);

}








void TIMER0_IRQHandler (void)
{
 	static int score_threshold=50;
	
	if(points>score_threshold)
	{
	score_threshold+=20;
	speed++;
	}
	
	
	//disabilito i timer se nell'iterazione di prima hanno fatto hit
	if(timer2en)
	{
	disable_timer(2);
	timer2en=0;
	}
	

	
	
	
  switch(direction)
	{
		case DOWN_LEFT:
											ndown_nleft(1,1,DOWN_RIGHT);

			break;
									
		case UP_LEFT:
									nup_nleft(1,1,UP_RIGHT,DOWN_LEFT);

      break;		
			
		case UP_RIGHT:
									nup_nright(1,1,UP_LEFT,DOWN_RIGHT);
		
		break;				
		case DOWN_RIGHT:
									ndown_nright(1,1,DOWN_LEFT);
		break;
		
									
									
		case DOWN_2LEFT:
									ndown_nleft(1,2,DOWN_2RIGHT);
									
			
			break;
		case DOWN_2RIGHT:
									ndown_nright(1,2,DOWN_2LEFT);
									
			
			break;
		case UP_2LEFT:
									nup_nleft(1,2,UP_2RIGHT,DOWN_2LEFT);
									
			break;
		case UP_2RIGHT:
									nup_nright(1,2,UP_2LEFT,DOWN_2RIGHT);
		
		break;
		
		case _2DOWN_LEFT:
									ndown_nleft(2,1,_2DOWN_RIGHT);
		
		break;
		
		case _2DOWN_RIGHT:
									ndown_nright(2,1,_2DOWN_LEFT);
		
		break;
		
		case _2UP_LEFT:
								nup_nleft(2,1,_2UP_RIGHT,_2DOWN_LEFT);
		break;
		
		case _2UP_RIGHT:
								nup_nright(2,1,_2UP_LEFT,_2DOWN_RIGHT);
		
		
									default:
										break;
	
	}

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}





/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	char str[10];
	
	
	static int down=0;
	static int pause=0;
	
	down++;
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0 && key== 1 ){         //eint1
		switch(down){
			case 1:
			
			init_RIT(0x004C4B40);									/* RIT Initialization 50 msec      passato  	*/
			enable_RIT();													/* RIT enabled												*/
     			
			init_timer(0, 0x1312D0 ); 						//avvio il gioco
			enable_timer(0);

			default:
				break;
		}
	}
 else if((LPC_GPIO2->FIOPIN & (1<<12)) == 0 && key==2){      //eint 2
			switch(down){
				case 1:
					
					if(pause==0)
					{
						disable_timer(0);
						disable_RIT();
						pause=1;
					}
					else
					{
					enable_RIT();
					enable_timer(0);
					pause=0;
					}
					
				
				break;
				default:
				break;
		}
	}	
	else if((LPC_GPIO2->FIOPIN & (1<<10)) == 0 && key==0){       //eint0
		
		switch(down){
			case 1:
			disable_timer(0);
			LCD_Clear(Black);
			LCD_Bordi(5);
			LCD_Paddle(64,10,90,320-32);
			points=0;
			speed=2;
			sprintf(str,"%d",points);
			GUI_Text(6,160,(uint8_t *) str,Green,Black);
			sprintf(str,"%d",record);
			GUI_Text(200,15,(uint8_t*) str,Green,Black);
			LCD_Ball(232,160);
			direction=DOWN_LEFT;
			
				break;
			default:
				break;
		}
	}
	
	else {	/* button released */
		down=0;		
		disable_timer(1);
		NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
		NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
		NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		key=-1;

	}
		
			LPC_TIM1->IR |= 0x1;			/* clear interrupt flag */

  return;
}






void TIMER2_IRQHandler (void)
{
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = SinTable[ticks]<<6;
	ticks++;
	if(ticks==45) 
	{ticks=0;
	enable_timer(0);
	}


LPC_TIM2->IR |= 0x1;          /* clear interrupt flag*/
}

/******************************************************************************
**                            End Of File
******************************************************************************/

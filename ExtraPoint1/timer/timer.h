/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_timer, funct_timer, IRQ_timer .c files
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

/* init_timer.c */
extern uint32_t init_timer( uint8_t timer_num, uint32_t timerInterval );
extern void enable_timer( uint8_t timer_num );
extern void disable_timer( uint8_t timer_num );
extern void reset_timer( uint8_t timer_num );
/* IRQ_timer.c */
extern void TIMER0_IRQHandler (void);
extern void TIMER1_IRQHandler (void);

#define UP_LEFT       0
#define _2UP_LEFT     1
#define UP_2LEFT      2

#define UP_RIGHT      3
#define _2UP_RIGHT    4
#define UP_2RIGHT     5


#define DOWN_RIGHT    6
#define _2DOWN_RIGHT  7
#define DOWN_2RIGHT   8

#define DOWN_LEFT     9
#define _2DOWN_LEFT   10
#define DOWN_2LEFT    11





#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/

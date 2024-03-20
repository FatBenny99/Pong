/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../GLCD/GLCD.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/
extern int toprightx,toprighty;
extern int bottomleftx,bottomlefty;

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

void ADC_IRQHandler(void) {
  	
	static int counter=0;
	int i;
	int numpos;
	numpos=10;
	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  
	if(abs(AD_current - AD_last) > 100)
	{
		counter++;
		if(counter>2)
		{
		if(AD_current>AD_last && AD_current!=AD_last)  //se il potenziometro è aumentato, muovo la barra a destra di 3 pixel
	{
		//le sovrascrivo nere solo se non sono arrivato al bordo
		if(toprightx<=240-5-numpos)
		{
			for(i=0;i<numpos;i++)
			LCD_DrawLine(bottomleftx+i,toprighty,bottomleftx+i,bottomlefty,Black);
			
		//scrivo le nuove colonne verdi
			for(i=0;i<numpos;i++)
			LCD_DrawLine(toprightx+1+i,toprighty,toprightx+1+i,bottomlefty,Green);
			
			
			//aggiorno i valori del paddle
			toprightx=toprightx+i;
			bottomleftx=bottomleftx+i;
		}
	}
		else if(  AD_current<AD_last && AD_current!=AD_last)                  //nel caso è diminuito lo muovo a sinistra
	{
	   if(bottomleftx>=0+5+numpos)
		 {
			 //cancello le colonne a destra
			 for(i=0;i<numpos;i++)
		 LCD_DrawLine(toprightx-i,toprighty,toprightx-i,bottomlefty,Black);

			 
			 //scrivo quelle a sinistra
			 for(i=0;i<numpos;i++)
			LCD_DrawLine(bottomleftx-i-1,toprighty,bottomleftx-i-1,bottomlefty,Green);

			 toprightx=toprightx-i;
			 bottomleftx=bottomleftx-i;
			}
		
		}
		counter=0;
	}
			
}
	AD_last = AD_current;
}

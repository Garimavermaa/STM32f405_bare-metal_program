/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#include "stm32f405xx.h"
#include "lcd.h"
//#include "lcd.c"

//-----------------------------------------------//
void delayms(uint32_t dly)
{
  uint32_t i,j=0;
  for(i=0;i<dly;i++)
  for(j=0;j<16000;j++);
}

//-----------------------------------------------//

//-----------------------------------------------//
uint32_t  result;
int main(void)
{

	 LcdInit();

	//PC2 ANALOG POT1
	//PC1 ANALOG POT2
	    /* set up pin PC6 for LED */

	    RCC->AHB1ENR |=  (1<<2);	            /* enable GPIOC clock */
	    GPIOC->MODER &= ~(3<<12);    /* clear pin mode for PC6 LED*/
	    GPIOC->MODER |=  (1<<12);    /* set pin to output mode -01 set bit 12 */


	    // set up pin PC1 for analog input
	    //RCC->AHB1ENR |=  (1<<2);
	    GPIOC->MODER &= ~(3<<2);    // clear pin mode PC1 -bit 3 and 2
	    GPIOC->MODER |= (3<<2);    //set pin to ANALOG mode -11,setting bit 2 and 3

	    /* setup ADC1 */
	    RCC->APB2ENR |= (1<<8);     //Enable ADC1 clock -ADC1 is connected to APB2 Bus(84Mhz)
	    ADC1->CR2 = 0;              //0: Disable ADC conversion and go to power down mode
	    //The total number of conversions in the regular group must be written in the L[3:0] bits in the
	   // ADC_SQR1 register
	    //ADC regular sequence register 3
	    ADC1->SQR1 &=~(0xF<<20); //Clearing bits 20-23, conversion sequence length is 1
	    //SQR3 -its 4:0 SQ1[4:0]: 1st conversion in regular sequence
	    ADC1->SQR3 |=(0xB<<0); //11   - conversion sequence and channel selection,Channel is 11 ADC1_CH11 for PC1
	    //SQR1 is 0000: 1 conversion
	    ADC1->CR1|=(1<<8);//Scan mode enable
	    ADC1->CR1 |=0xB;   //01100  -Channel 11
	    ADC1->CR2 |= 1;                 // Enable ADC

	    while (1) {
	        ADC1->CR2 |= (1<<30);        /* start a conversion at regular channel*/
	        while(!(ADC1->SR & 2)) {}       // wait for conv complete -monitor bit 1
	        result = ADC1->DR; /* read conversion result */

	        lprint(0x80,"ADC Value-");

	        		aprint(result);
	        		for(uint32_t i=0;i<50000;i++);
	        if (result >100)

	        	GPIOC->ODR &= ~( 1 << 6);//PC6 LED ON

	        else

	        GPIOC->ODR|= ( 1 << 6); //PC6 =1 // TURN OFF LED
	    }

  return(0);
}




#include "stm32f4xx.h"
#include <stdint.h>


uint16_t count=0;

void EXTI3_IRQHandler(void)
{
    if (((EXTI->PR)>>3) & 1){

    	count=count+1;
    	//GPIOB->ODR ^= (1 <<13);
    	for(int j=0; j<10000; j++);
        EXTI->PR = (1 << 3);
    }
}
int main(void)
{
	uint16_t xcount=0;

    RCC->AHB1ENR |= (3 << 1);   // enable B and C ports clocks

    GPIOB->MODER &=~(3<<26);
    GPIOB->MODER &=~(3<<28);
    GPIOB->MODER &=~(3<<30);
    GPIOC->MODER &=~(1<<12);   // PC6

    GPIOB->MODER |=(1<<26);
    GPIOB->MODER |=(1<<28);
    GPIOB->MODER |=(1<<30);
    GPIOC->MODER |=(1<<12);   // make PC6


    GPIOB->MODER &= ~(3<<6);  // Interrupt pin has to be in INPUT mode


    RCC->APB2ENR |= (1 << 14);

    SYSCFG->EXTICR[0] |= (1<<12);

    EXTI->RTSR |= (1<<3);

    EXTI->IMR |= (1<<3);

    NVIC_SetPriority(EXTI3_IRQn, 1);

    NVIC_EnableIRQ(EXTI3_IRQn);

// =================================

    while(1)
    {
       if (xcount != count)  // if both are not same, then it means count has changed
       {
    	 if (count & 1)   // check LSB of count
    	   GPIOC->ODR &= ~(1<<6);    // clear PC6 so LED will glow
    	 else
    	   GPIOC->ODR |= (1<<6);   // set so LED will not glow

    	 if (count & 2)  // next bit
           GPIOB->ODR &= ~(1<<13);    // PB13 (LED2) ON
    	 else
    	  GPIOB->ODR |= (1<<13);

    	 if (count & 4)  // next bit
           GPIOB->ODR &= ~(1<<14);    // PB14 (LED3) ON
    	 else
    	  GPIOB->ODR |= (1<<14);

    	 if (count & 8)  // next bit
           GPIOB->ODR &= ~(1<<15);    // PB15 (LED3) ON
    	 else
    	  GPIOB->ODR |= (1<<15);
         xcount = count;
       }
    }

    return 0;
}

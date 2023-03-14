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
#include "stm32f4xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


/* It is recommended to enable slave before master starts sending clock*/
void spi_slaveconfig()
{
	RCC->APB1ENR |= (1<<14); //SPI2 clock enable
	SPI2->CR1 &=~(1<<11);	//8-bit data format
	SPI2->CR1	|= (1<<0)|(1<<1);	//CPHA=1, 	CPOL=1
	SPI2->CR1 &=~(1<<7);	//MSB bit first
	SPI2->CR1 |= (1<<9); 	//Software slave enable
	SPI2->CR1 &= ~(1<<8);	//clear Internal slave select
	SPI2->CR1 &= ~(1<<2);	//Slave mode enable
	SPI2->CR1 &=~(1<<10);	//Full duplex mode
	SPI2->CR1 &=~(3<<12);	// No CRC
	SPI2->CR1 &=~(1<<15);	// unidirectional mode
	SPI2->CR1 |= (1<<6);	// SPI Enable
}


void spi_masterconfig()
{
	RCC->APB2ENR |= (1<<12);	// Enable SPI1 clock
	SPI1->CR1 &= ~(7<<3);	//clear baudrate
	SPI1->CR1 |= (3<<3); 	//set baudrate by fclk/16
	SPI1->CR1 |= (1<<0)|(1<<1);	//CPHA=1, 	CPOL=1
	SPI1->CR1 &=~(1<<11);	//8-bit data format
	SPI1->CR1 &=~(1<<7);	//MSB bit first
	SPI1->CR1 |= (1<<9); 	//Software slave enable
	SPI1->CR1 |= (1<<8);	//Internal slave select
	SPI1->CR1 &=~(1<<10);	//Full duplex mode
	SPI1->CR1 |= (1<<2);		//Master Enable
	SPI1->CR1 &=~(3<<12);	// No CRC
	SPI1->CR1 &=~(1<<15);	// unidirectional mode
	SPI1->CR1 |= (1<<6);	// SPI Enable
}


void spi_gpioconfig()
{
	/*SPI 1 GPIO pins*/
	/* PA5(21)--> SCK  */
	/* PA6(22)--> MISO  */
	/* PA7(23) --> MOSI  */
	RCC->AHB1ENR |= (1<<0);	//PortA Clock Enable
	GPIOA->MODER &= ~((3<<10)|(3<<12)|(3<<14));	//clear PA5-PA6
	GPIOA->MODER |= (2<<10)|(2<<12)|(2<<14);	//Alternate mode
	GPIOA->OTYPER &= ~(7<<5); 	//push-pull for PA5-PA7;
	GPIOA->OSPEEDR &= ~((3<<10)|(3<<12)|(3<<14));	//clear speed select
	GPIOA->OSPEEDR |= (2<<10)|(2<<12)|(2<<14);	//High speed PA5-PA7
	GPIOA->AFR[0] &= ~(0xFFF<<20);
	GPIOA->AFR[0] |= (0x555<<20);	//SPI alternate function PA5-PA7
	/*SPI 2 GPIO pins*/
	/* PB13 (34)--> SCK  */
	/* PB14 (35)--> MISO  */
	/* PB15 (36)--> MOSI  */
	RCC->AHB1ENR |= (1<<1);	//PortB Clock Enable
	GPIOB->MODER &= ~((3<<26)|(3<<28)|(3<<30));	//clear PB13-PB15
	GPIOB->MODER |= (2<<26)|(2<<28)|(2<<30);	//Alternate mode
	GPIOB->OTYPER &= ~(7<<13); 	//push-pull for PB13-PB15;
	GPIOB->OSPEEDR &= ~((3<<26)|(3<<28)|(3<<30));	//clear speed select
	GPIOB->OSPEEDR |= (2<<26)|(2<<28)|(2<<30);	//High speed PB13-PB15
	GPIOB->AFR[1] &= ~(0xFFF<<20);
	GPIOB->AFR[1] |= (0x555<<20);	//SPI alternate function PB13-PB15
}
void led_config()
{
	RCC->AHB1ENR |=(1<<2);	//Enable PORT C clock
	GPIOC->MODER &= ~(1<<12);
	GPIOC->MODER |= (1<<12);	//PC6 as output
	GPIOC->OTYPER &= ~(1<<6);	//push pull at PC6
	GPIOC->OSPEEDR &= ~(3<<12);	//low speed
	GPIOC->ODR |= (1<<6);		// LED OFF
}

void switch_config()
{
	RCC->AHB1ENR |= (1<<1);		//enable GPIOB clock
	GPIOB->MODER &= ~(3<<14);	// Input mode PB7
	GPIOB->MODER &= ~(3<<6);	// Input mOde PB3
	RCC->APB2ENR |= (1<<14);	//SYSCFG enable
	SYSCFG->EXTICR[1] |= (1<<12);		// Enable External interrupt in PB7
	SYSCFG->EXTICR[0] |= (1<<12);		// Enable external interrupt in PB3
	EXTI->IMR |= (1<<7)|(1<<3);		// Line 7 & 3 of External Interrupt not masked
	EXTI->FTSR |= (1<<7)|(1<<3);		// Falling edge Interrupt enable for PB7, PB3
//	NVIC_SetPriority(EXTI9_5_IRQn,1);	// Set priority to External Interrupt
//	NVIC_SetPriority(EXTI3_IRQn,1);	// Set priority to External Interrupt PB3
	NVIC_EnableIRQ(EXTI9_5_IRQn); // Enable External Interrupt for pin 5-9
	NVIC_EnableIRQ(EXTI3_IRQn); // Enable External Interrupt for pin PB3
}
/*Since we using PB7- 7th GPIO pin we need to use this IRQn*/
void EXTI9_5_IRQHandler()
{
	if(EXTI->PR & (1<<7))		// check for Interrupt trigger occurs
	{
		SPI1->DR = 1;/* send data from SPI-1 */
		    			/* wait unit data is transmitted */
		while (!(SPI1->SR & (1<<1)));
		     			/*Data transmitted*/
		EXTI->PR |= (1<<7);		// Set to clear the flag
	}
}

/*PB3 is used for External interrupt so need to use EXTI3_IRQn*/
void EXTI3_IRQHandler()
{
	if(EXTI->PR & (1<<3))		// check for Interrupt trigger occurs
	{
		SPI1->DR = 0;/* send data from SPI-1 */
		    			/* wait unit data is transmitted */
		while (!(SPI1->SR & (1<<1)));
		     			/*Data transmitted*/
		EXTI->PR |= (1<<3);		// Set to clear the flag
	}
}

int main(void)
{
    led_config();
    switch_config();
    spi_gpioconfig();
    spi_slaveconfig();
    spi_masterconfig();
    for (uint32_t i=0; i<1000; i++);
    while (1)
    {
  	     if (SPI2->SR & (1<<0))
  	     {
  	    	 /*if data received in SPI)*/
  	    	 if (SPI2->DR == 0)
  	    	 {
  	    		 GPIOC->ODR |= (1 << 6);
   	         }
  	    	 if (SPI2->DR == 1)
  	    	 {
  	    	  	 GPIOC->ODR &= ~(1 << 6);
  	    	 }

   	     }

   	}
}

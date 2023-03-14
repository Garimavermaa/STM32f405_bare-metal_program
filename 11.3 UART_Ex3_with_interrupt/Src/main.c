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


/******PROCEDURE************/
/* Step 1: Identify the bus clock , APB1 bus (UART4), AHB1 (PORTA)*/
/*Step 2: Config PORTA for UART4 PA0-> TX, PA1->RX*/
/* 2.1-> Select Mode of operation for PA0 & PA1 as alternate fn*/
/* 2.2-> AFRL alternate function as AF8 (PA0 & PA1)*/
/*2.4-> select speed as high speed in PA0 & PA1*/
/*Step 3: config UART*/

#define ON 1
#define OFF 0
#define CLEAR 2
char rx_data[10];
uint8_t ind=0;
uint8_t status =CLEAR;
void swt_config()
{
		RCC->AHB1ENR |= (1<<1);		//enable GPIOB clock
		GPIOB->MODER &= ~(3<<14);	// Input mode PB7
		GPIOB->MODER &= ~(3<<6);	// Input mOde PB3
		RCC->APB2ENR |= (1<<14);	//SYSCFG enable
		SYSCFG->EXTICR[1] |= (1<<12);		// Enable External interrupt in PB7
		SYSCFG->EXTICR[0] |= (1<<12);		// Enable external interrupt in PB3
		EXTI->IMR |= (1<<7)|(1<<3);		// Line 7 & 3 of External Interrupt not masked
		EXTI->FTSR |= (1<<7)|(1<<3);		// Falling edge Interrupt enable for PB7, PB3
		NVIC_EnableIRQ(EXTI9_5_IRQn); // Enable External Interrupt for pin 5-9
		NVIC_EnableIRQ(EXTI3_IRQn); // Enable External Interrupt for pin PB3
}
void UART4_config()
{
	/*UART port pin config*/
	RCC->AHB1ENR |= (1<<0);			// enable GPIOA clock
	GPIOA->MODER &= ~(0xF<<0);		// enable alternate fn in PA0 & 1
	GPIOA->MODER |=(0xA<<0);
	GPIOA->AFR[0] &= ~(0xFF<<0);	// enable UART4 TX & RX function for PA0 & PA1
	GPIOA->AFR[0] |= (0x88<<0);
	GPIOA->OSPEEDR |= (0xF<<0);		// set output speed as high speed
	/*UART4 config */
	RCC->APB1ENR |= (1<<19); 		// enable UART4 clock
	UART4->CR1 |= (1<<13); 			//Uart4 is enabled
	UART4->CR1 &= ~(1<<12); 		//set 8bit data mode
 	UART4->BRR = 0x683;				// baudrate for 9600
 	UART4->CR1 |= (1<<5); 			//Uart4 Rx interrupt enable
 	UART4->CR1 |= (3<<2); 			// TE & RE enable
 	NVIC_EnableIRQ(UART4_IRQn);		// UART4 Rx interrupt enable in NVIC
}

void UART4_sendchar(uint8_t x)
{
	UART4->DR = x;						/*Please the data in DR register for transmitting*/
	while(!(UART4->SR & (1<<6)));		/*Wait until TC (Transmission complete) is 1*/
}

void UART4_sendstring(char *string)
{
	while(*string)
	{
		UART4_sendchar(*string++);
	}
}

void UART4_IRQHandler()
{
	//while(!(UART4->SR & (1<<5)));		/*wait until RXNE bit is set which indicates data receive*/
	rx_data[ind]=UART4->DR;
	ind++;
	rx_data[ind]='\0';
}

uint8_t stringcmp(const char *string1, const char *string2)
{
	uint8_t flag=1,i=0;
	while(string1[i]!='\0'&& string2[i]!='\0')
	{
		if(string1[i]!=string2[i])
		{
			flag=1;
			break;
		}
		i++;
		flag=0;
	}
	if(flag==0)
		return 0;
	else
		return 1;
}

/*Since we using PB7- 7th GPIO pin we need to use this IRQn*/
void EXTI9_5_IRQHandler()
{
	if(EXTI->PR & (1<<7))		// check for Interrupt trigger occurs
	{
		status=ON;
		EXTI->PR |= (1<<7);		// Set to clear the flag
	}
}

/*PB3 is used for External interrupt so need to use EXTI3_IRQn*/
void EXTI3_IRQHandler()
{
	if(EXTI->PR & (1<<3))		// check for Interrupt trigger occurs
	{
		status=OFF;
		EXTI->PR |= (1<<3);		// Set to clear the flag
	}
}

void led_config()
{
	RCC->AHB1ENR |= (1<<2);			// Enable clock for GPIOC
	GPIOC->MODER &= ~(3<<12);		// Set PC6 as Output Mode
	GPIOC->MODER |= (1<<12);
	GPIOC->OTYPER &= ~(1<<6);		// PC6 as push pull output
	GPIOC->OSPEEDR &= ~(3<<12);		// PC6 as low speed
	GPIOC->ODR |= (1<<6);			// LED OFF
}

void check_ledstatus()
{
	if(status==ON)
	{
		UART4_sendstring("ON");	// Send '1' in UART4
		status=CLEAR;
	}
	else if(status==OFF)
	{
		UART4_sendstring("OFF");	// Send '0' in UART4
		status=CLEAR;
	}
	if(!(stringcmp(rx_data,"ON")))			// if '1' receive LED ON
	{
		GPIOC->ODR &= ~(1<<6);
	}
	else if (!(stringcmp(rx_data,"OFF")))			// if '0' receive LED OFF
	{
		GPIOC->ODR |= (1<<6);
	}

	for(uint32_t d=0;d<100000;d++);
	for(uint8_t i=0;i<10;i++)
		{
			rx_data[i] = '\0';
		}
		ind=0;

}

int main(void)
{
	UART4_config();
	led_config();
	swt_config();
	for(;;)
	{
		check_ledstatus();
	}
}

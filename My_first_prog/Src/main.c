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
#include "gpio405.h"
#include "lcd.h"

#include <stdio.h>
#include <stdlib.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
  uint16_t count = 0;
  char buf[20];
	LcdInit ();
	//lprint (0x80,"Hello");

	while (1)
	{
	  sprintf (buf,"Count -> %04d", count++);
	  lprint (0x80,buf);
	  gen_delay (10000);
	 }

//	RCC_AHB1ENR |= 7;  // page 242 RM90

// do not use the following bits
//	GPIOB_MODER &= ~(3<<30);  // gpiox_moder
//	GPIOB_MODER |= (1<<30);
//	GPIOB_OTYPER &= ~(1<<15);
//	GPIOB_OSPEEDR &= ~(3<<30);
//

//    /* Loop forever */
//	for(;;)
//	{
//		// Type 1
//		GPIOB_ODR &= ~(1<<15);
//		for (uint32_t i = 0; i<900000; i++);
//		GPIOB_ODR |= (1<<15);
//		for (uint32_t i = 0; i<300000; i++);
//
//	}
}

void gen_delay (int dly)
{
 while (dly) dly--;
}

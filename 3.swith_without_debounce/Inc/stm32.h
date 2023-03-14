/*
 * stm32.h
 *
 *  Created on: Feb 17, 2023
 *      Author: garim
 */

#ifndef STM32_H_
#define STM32_H_


#define RCC 0x40023800
#define GPIOB 0x40020400
#define GPIOC 0x40020800

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC+0x30)

#define GPIOB_MODER *(volatile uint32_t*)(GPIOB+0x00)
#define GPIOB_ODR *(volatile uint32_t*)(GPIOB+0x14)
#define GPIOB_IDR *(volatile uint32_t*)(GPIOB+0x10)


#define GPIOC_MODER *(volatile uint32_t*)(GPIOC + 0x00)
#define GPIOC_ODR 	*(volatile uint32_t*)(GPIOC + 0x14)
//port c



//#endif /* STM32_H_ */


#endif /* STM32_H_ */

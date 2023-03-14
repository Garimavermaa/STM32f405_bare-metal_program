/*
 * gpio405.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Garima
 */

#ifndef GPIO405_H_
#define GPIO405_H_


#define RCC 0x40023800
#define GPIOC 0x40020800
#define TIM3 0x40000400
//RCC Related Registers
#define RCC_AHB1ENR *(volatile uint32_t*)(RCC+0x30)
#define RCC_APB1ENR *(volatile uint32_t*)(RCC+0x40)
//GPIOC Related Registers
#define GPIOC_MODER *(volatile uint32_t*)(GPIOC+0x00)
#define GPIOC_OTYPER *(volatile uint32_t*)(GPIOC+0x04)
#define GPIOC_OSPEEDR *(volatile uint32_t*)(GPIOC+0x08)
#define GPIOC_ODR *(volatile uint32_t*)(GPIOC+0x14)
#define GPIOC_AFRL *(volatile uint32_t*)(GPIOC+0x20)

//TIM3 related registers
#define TIM3_PSC *(volatile uint32_t*)(TIM3+0x28)
#define TIM3_ARR *(volatile uint32_t*)(TIM3+0x2C)
#define TIM3_CNT *(volatile uint32_t*)(TIM3+0x24)
#define TIM3_CR1 *(volatile uint32_t*)(TIM3+0x00)
#define TIM3_SR *(volatile uint32_t*)(TIM3+0x10)
#define TIM3_CCMR1 *(volatile uint32_t*)(TIM3+0x18)
#define TIM3_CCER *(volatile uint32_t*)(TIM3+0x20)
#define TIM3_CCR1 *(volatile uint32_t*)(TIM3+0x34)
#define TIM3_EGR *(volatile uint32_t*)(TIM3+0x14)


#endif /* GPIO405_H_ */

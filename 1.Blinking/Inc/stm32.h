/*
 * gpio405.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Garima
 */

#ifndef GPIO405_H_
#define GPIO405_H_


#define RCC 0x40023800
#define GPIOB 0x40020400

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC+0x30)

#define GPIOB_MODER *(volatile uint32_t*)(GPIOB+0x00)
#define GPIOB_OTYPER *(volatile uint32_t*)(GPIOB+0x04)
#define GPIOB_OSPEEDR *(volatile uint32_t*)(GPIOB+0x08)
#define GPIOB_ODR *(volatile uint32_t*)(GPIOB+0x14)

#endif /* GPIO405_H_ */







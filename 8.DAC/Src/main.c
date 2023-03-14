
//DAC Output we can take from pin PA4(From Data sheet)
//Connect a wire between PA4 and PC6. In PC6 LED We can get the sine wave signal
//The LED gradually TURN ON and OFF depending on the digital values given in the sine wave array

//PA4 -Pin 20 (x5), PC6 -pin 37 (x8)

#include "stm32f405xx.h"
uint32_t analog;
void delayms(uint32_t dly)
{
  uint32_t i,j=0;
  for(i=0;i<dly;i++)
  for(j=0;j<16000;j++);
}
int main(void) {
int i;
 const static int sineWave[] =
        {2048,3071,3821,4095,3821,3071,2048,1024,274,0,274,1024};

    //PA4 in Analog mode
    RCC->AHB1ENR |=  (1<<0);	 //enable GPIOA clock Datasheet 19
    GPIOA->MODER &= ~(3<<8);    //clear pin mode for PA4 DAC PIN
    GPIOA->MODER |=  (3<<8);    //PA4 in analog mode

//PC6 IN ANALOG MODE
    RCC->AHB1ENR |=  (1<<2);	            /* enable GPIOC clock */
    GPIOC->MODER &= ~(3<<12);    /* clear pin mode for PC6 LED RM 281*/
    GPIOC->MODER |=  (3<<12);    //PC6 in analog mode

    /* setup DAC */
    RCC->APB1ENR |= (1 << 29);    //Enable DAC clock	RM 183
    DAC->CR |= (1<<0);             // Enable DAC Rm 445

    while(1) {
        for (i = 0; i < sizeof(sineWave)/sizeof(int); i++)
        {

            DAC->DHR12R1 = sineWave[i]; /* write value of sinewave to DAC RM448*/

            DAC->SWTRIGR |=(1<<0);// Trigger ch1.
			analog=DAC->DHR12R1 ;//Check variable in Live expression-Debug mode
            delayms(20);
        }
    }
}

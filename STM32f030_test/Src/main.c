// Simon_test
#include "stm32f030x6.h"
#include <stdint.h>






void SystemClock_Config(void);
void GPIO_Init();

int main(void)
{


  SystemClock_Config();
  GPIO_Init();
  SysTick->CTRL |= (0x01);
  SysTick->LOAD |= (0x00FFFFFF);

  uint32_t lastTimerValue = SysTick->VAL;
  uint32_t currentTimerValue;

  while (1)
  {
	  currentTimerValue = SysTick->VAL;
	  if(lastTimerValue - currentTimerValue >= 1000*1000){
		  lastTimerValue = currentTimerValue;
		  GPIOC->ODR ^= (1 << 14);
	   	  }

  }

}


void SystemClock_Config(void)
{
	RCC->CR |= (1);
	RCC->AHBENR |= (1<<19);

}

void GPIO_Init(){

	GPIOC->MODER |= (1<<28);


}


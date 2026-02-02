// Simon_test
#include "stm32f103x6.h"
#include <stdint.h>






void SystemClock_Config(void);
void GPIO_Init();

int main(void)
{


  SystemClock_Config();
  GPIO_Init();
  SysTick->CTRL |= (0x01);
  SysTick->LOAD |= (0x00FFFFFF);

  uint32_t lastTimerValue = *(volatile uint32_t*)(0xE000E010+0x08);
  uint32_t currentTimerValue;

  while (1)
  {
	  currentTimerValue = SysTick->VAL;
	  if(lastTimerValue - currentTimerValue >= 1000*1000){
		  lastTimerValue = currentTimerValue;
		  GPIOC->ODR ^= (0x01 << 13);
	   	  }

  }

}


void SystemClock_Config(void)
{
	RCC->CR |= (1);
	RCC->APB2ENR |= (1<<4)|(1<<2)|(1);

}

void GPIO_Init(){

	GPIOC->CRH &= ~(0x03 << 22);
	GPIOC->CRH ^= (0x02 << 20);

}


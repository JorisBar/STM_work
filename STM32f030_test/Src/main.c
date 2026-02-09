// Simon_test
#include "stm32f030x6.h"
#include <stdint.h>






void SystemClock_Config(void);
void GPIO_Init();
void EXTI_Init();
void EXTI_4_15_CallBack();

volatile uint16_t period;

int main(void)
{


  SystemClock_Config();
  GPIO_Init();
  EXTI_Init();
  SysTick->CTRL |= (0x01);
  SysTick->LOAD |= (0x00FFFFFF);

  uint32_t lastTimerValue = SysTick->VAL;
  uint32_t currentTimerValue;
  period = 1000;
  while (1)
  {

	  currentTimerValue = SysTick->VAL;
	  if(lastTimerValue - currentTimerValue >= 1000*period){
		  lastTimerValue = currentTimerValue;
		  GPIOC->ODR ^= (1 << 14);
	   	  }


  }

}


void SystemClock_Config(void)
{
	RCC->CR |= (1);
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

}

void GPIO_Init(){

	GPIOC->MODER |= (1 << 28);
	GPIOC->PUPDR |= (1 << 30);


}
void EXTI_Init(){

	EXTI->IMR |= (1 << 15);
	EXTI->FTSR |= (1 << 15);

	SYSCFG->EXTICR[3] |= (1 << 13);
	EXTI->PR = (1<<15);

	NVIC_EnableIRQ(EXTI4_15_IRQn);

}

void EXTI4_15_IRQHandler(){

	EXTI->PR = (1<<15);
	EXTI_4_15_CallBack();

}

void EXTI_4_15_CallBack(){
	period /= 2;

}




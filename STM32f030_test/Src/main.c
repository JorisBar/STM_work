// Simon_test
#include "stm32f030x6.h"
#include <stdint.h>





uint8_t new_bcd_time(uint8_t delay);
void SystemClock_Config(void);
void GPIO_Init();
void EXTI_Init();
void RTC_Init();
void EXTI_4_15_CallBack();
void RTC_CallBack();

volatile uint16_t period;
volatile uint8_t bcd;
volatile uint8_t seconds;




int main(void)
{

volatile uint32_t debug_ALRMAR;
volatile uint32_t debug_TR;
  SystemClock_Config();
  GPIO_Init();
  EXTI_Init();
  RTC_Init();
  SysTick->CTRL |= (0x01);
  SysTick->LOAD |= (0x00FFFFFF);

  uint32_t lastTimerValue = SysTick->VAL;
  uint32_t currentTimerValue;
  period = 1000;

  while (1)
  {
	  /*
	  currentTimerValue = SysTick->VAL;
	  if(lastTimerValue - currentTimerValue >= 1000*period){
		  lastTimerValue = currentTimerValue;
		  GPIOC->ODR ^= (1 << 14);
	   	  }
	*/

	  debug_ALRMAR = RTC->ALRMAR;
	  debug_TR = RTC->TR;

  }

}


void SystemClock_Config(void)
{
	RCC->CR |= (1);
	RCC->CSR |= RCC_CSR_LSION;
	while (!(RCC->CSR & RCC_CSR_LSIRDY));
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSI;
	RCC->BDCR |= RCC_BDCR_RTCEN;

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
	GPIOC->ODR |= (1 << 14);

}

void RTC_Init(){
	//debug_ALRMAR = RTC->ALRMAR;
	//debug_TR = RTC->TR;

	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	RTC->ISR |= RTC_ISR_ALRAF;

	RTC->CR &= ~RTC_CR_ALRAE;
	while(!(RTC->ISR & RTC_ISR_ALRAWF));

	//debug_ALRMAR = RTC->ALRMAR;
	//debug_TR = RTC->TR;

	RTC->PRER = (0x7F << RTC_PRER_PREDIV_A_Pos);
	RTC->PRER |= (0xFF << RTC_PRER_PREDIV_S_Pos);

	RTC->ISR &= ~RTC_ISR_RSF;
	while (!(RTC->ISR & RTC_ISR_RSF));


	RTC->ALRMAR |= RTC_ALRMAR_MSK4_Msk|RTC_ALRMAR_MSK3_Msk|RTC_ALRMAR_MSK2_Msk;

	RTC->ALRMAR &= ~(RTC_ALRMAR_ST_Msk|RTC_ALRMAR_SU_Msk);
	RTC->ALRMAR |= new_bcd_time(1);




	RTC->ALRMASSR |= (8 << RTC_ALRMASSR_MASKSS_Pos);

	RTC->ALRMASSR = (RTC->ALRMASSR & RTC_ALRMASSR_MASKSS_Msk) | (RTC->SSR & RTC_ALRMASSR_SS_Msk);





	RTC->CR |= RTC_CR_ALRAE;

	NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_IRQHandler(){
	RTC->ISR |= RTC_ISR_ALRAF;

	RTC_CallBack();


}
void RTC_CallBack(){

	RTC->CR &= ~RTC_CR_ALRAE;
	while(!(RTC->ISR & RTC_ISR_ALRAWF));

	RTC->ALRMAR &= ~(RTC_ALRMAR_ST_Msk|RTC_ALRMAR_SU_Msk);
	RTC->ALRMAR |= new_bcd_time(1);
	GPIOC->ODR ^= GPIO_ODR_14;
	RTC->CR |= RTC_CR_ALRAE;

}


uint8_t new_bcd_time(uint8_t delay){
	bcd = RTC->TR&(RTC_TR_ST_Msk|RTC_TR_SU_Msk);
	seconds = (bcd >> 4)*10 + (bcd & 0xF);
	seconds += delay;
	if(seconds==60){
		seconds=0;
	}
	return ((seconds/10) << 4)|(seconds % 10);
}




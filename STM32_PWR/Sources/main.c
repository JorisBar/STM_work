#include <stdint.h>



#define RCC_BASE 0x40021000
#define GPIOC_BASE 0x40011000
#define GPIOA_BASE 0x40010800
#define EXTI_BASE 0x40010400
#define NVIC_BASE 0xE000E100
#define PWR_BASE 0x40007000
#define RTC_BASE 0x40002800


void PWR_Config();
void SystemClock_Config(void);
static void GPIO_Init(void);
void EXTI0_CallBack();

volatile uint16_t delay = 500;

int main(void)
{


  SystemClock_Config();
  PWR_Config();
  *(volatile uint32_t*)(0xE000E010+0x00) |= (0x01);
  *(volatile uint32_t*)(0xE000E010+0x04) |= (0x00FFFFFF);

  GPIO_Init();



  uint32_t lastTimerValue = *(volatile uint32_t*)(0xE000E010+0x08);
  uint32_t currentTimerValue;
  uint32_t WKUPTimerValue;


   while (1)
   {
	  WKUPTimerValue = *(volatile uint32_t*)(0xE000E010+0x08);
	  currentTimerValue = *(volatile uint32_t*)(0xE000E010+0x08);
	  while(WKUPTimerValue - currentTimerValue < 1000*5000){
		  currentTimerValue = *(volatile uint32_t*)(0xE000E010+0x08);
		  if(lastTimerValue - currentTimerValue >= 1000*delay){
			  lastTimerValue = currentTimerValue;
			  *(volatile uint32_t*)(GPIOC_BASE+0x0C) ^= (1 << 13);
 	  }
	  }
 	 __asm volatile ("WFI");


  }

}


void SystemClock_Config(void)
{
	*(volatile uint32_t*)(RCC_BASE+0x00) |= (1);
	*(volatile uint32_t*)(RCC_BASE+0x18) |= (1<<4)|(1<<2)|(1);

	//RTC
	*(volatile uint32_t*)(RTC_BASE+0x00) |= (1<<1);
	while((*(volatile uint32_t*)(RTC_BASE+0x04)&(1<<5))=0){} //RTOFF
	*(volatile uint32_t*)(RTC_BASE+0x04) |= (1<<4);


}



static void GPIO_Init(void)
{
	// NVIC
	*(volatile uint32_t*)(NVIC_BASE+0x00) |= (1<<6);
	//*(volatile uint32_t*)(NVIC_BASE+0x304) |= (0x0A<<20); priority


	// EXTI
	*(volatile uint32_t*)(EXTI_BASE+0x00) |= (1);
	//*(volatile uint32_t*)(EXTI_BASE+0x08) |= (1); rising edge
	*(volatile uint32_t*)(EXTI_BASE+0x0C) |= (1);

	// GPIO
	*(volatile uint32_t*)(GPIOC_BASE+0x04) &= ~(0x03 << 22);
	*(volatile uint32_t*)(GPIOC_BASE+0x04) ^= (0x02 << 20);

}
void EXTI0_IRQHandler(){
	*(volatile uint32_t*)(EXTI_BASE+0x14) = (1);
	EXTI0_CallBack();
}

void EXTI0_CallBack(){
/*
	if(*(volatile uint32_t*)(GPIOA_BASE+0x08)&(1)){
		delay = 500;
	}
	else{
		delay = 250;
	}
	*/
}
void PWR_Config(){
	*(volatile uint32_t*)(PWR_BASE+0x00) |= (1);


}




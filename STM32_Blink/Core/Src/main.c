
#include "main.h"


#define RCC_BASE 0x40021000
#define GPIOC_BASE 0x40011000


void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{


  SystemClock_Config();
  *(volatile uint32_t*)(0xE000E010+0x00) |= (0x01);
  *(volatile uint32_t*)(0xE000E010+0x04) |= (0x00FFFFFF);

  MX_GPIO_Init();



  uint32_t lastTimerValue = *(volatile uint32_t*)(0xE000E010+0x08);
  uint32_t currentTimerValue;

   while (1)
   {
	  currentTimerValue = *(volatile uint32_t*)(0xE000E010+0x08);
 	  if(lastTimerValue - currentTimerValue >= 1000*1000){
 		  lastTimerValue = currentTimerValue;
 		  *(volatile uint32_t*)(GPIOC_BASE+0x0C) ^= (0x01 << 13);
 	  }

  }

}


void SystemClock_Config(void)
{
	*(volatile uint32_t*)(RCC_BASE+0x00) |= (1);
	*(volatile uint32_t*)(RCC_BASE+0x18) |= (1<<4);


}



static void MX_GPIO_Init(void)
{
	*(volatile uint32_t*)(GPIOC_BASE+0x04) &= ~(0x03 << 22);
	*(volatile uint32_t*)(GPIOC_BASE+0x04) ^= (0x02 << 20);

}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

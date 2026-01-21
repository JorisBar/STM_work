

#include <stdint.h>



#define RCC_BASE 0x40021000
#define GPIOC_BASE 0x40011000


void SystemClock_Config(void);
static void GPIO_Init(void);

int main(void)
{


  SystemClock_Config();


  GPIO_Init();


  uint16_t value;


   while (1)
   {
	   value =*(volatile uint32_t*)(GPIOC_BASE+0x08);
	   value &=(0x01<<15);
	   if(value==0){
		   *(volatile uint32_t*)(GPIOC_BASE+0x0C) &= ~(0x01 << 13);

	   }
	   else{
		   *(volatile uint32_t*)(GPIOC_BASE+0x0C) |= (0x01 << 13);

	   }
  }
}


void SystemClock_Config(void)
{
	*(volatile uint32_t*)(RCC_BASE+0x00) |= (1);
	*(volatile uint32_t*)(RCC_BASE+0x18) |= (1<<4);

}



static void GPIO_Init(void)
{
	*(volatile uint32_t*)(GPIOC_BASE+0x04) &= ~(0x02 << 22);
	*(volatile uint32_t*)(GPIOC_BASE+0x04) ^= (0x02 << 20);



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

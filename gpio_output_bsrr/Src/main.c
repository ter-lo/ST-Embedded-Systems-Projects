/*
 * gpio_output_bsrr
 *
 * Develop GPIO Output Driver
 * Control GPIO Outputs using bit set/reset register (BSRR)
 *
 * I included a header file and used project properties to
 * include chip header file like "stm32f0xx.h"
 *
 * Software for the files: https://www.st.com/en/embedded-software/stm32cubef0.html
 *
 * Directories needed:
 * CMSIS/include
 * CMSIS/Device/ST/STM32F0xx/Include
 *
 */

#include "stm32f0xx.h"

#define GPIOA_CLK_EN		(1U<<17)
#define USER_LED2_MODER		(1U<<10)
#define USER_LED2			(1U<<5)

int main(void)
{
		/* Enable clock access to port A */
		RCC->AHBENR |= GPIOA_CLK_EN;

		/* Configure LED Pin as output pins */
		GPIOA->MODER |= USER_LED2_MODER;


		/* Turn on LED using a while loop */
		while(1)
		{
			/* Turn on all LED (LD2) */
			// GPIOA->ODR ^= USER_LED2;

			/*
			 * Toggle LED (LD2) using XOR ^
			 */
			// GPIOA->ODR ^= USER_LED2;

			/*
			 * Toggle LED using BSRR (Bit set/reset register)
			 */
			GPIOA->BSRR ^= (1U<<5);

			for(int i = 0; i < 1000000; i++){}

			GPIOA->BSRR ^= (1U<<21);

		}


}

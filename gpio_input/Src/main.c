/*
 * gpio_input
 *
 * Use the user button to toggle LED on/off
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
 * To turn the LED off when the button is pressed
 * I had to define GPIOC_CLK_EN at bit 19 and
 * define PIN13 as on as described in UM1724 7.7
 * B1 USER is connected to I/O PC13 (Port C, Pin 13)
 *
 */

#include "stm32f0xx.h"

#define GPIOA_CLK_EN		(1U<<17)
#define GPIOC_CLK_EN		(1U<<19)
#define USER_LED2_MODER		(1U<<10)
#define USER_LED2			(1U<<5)

/* Button pin */
#define PIN13				(1U<<13)
#define BTN_PIN				PIN13

int main(void)
{
		/* Enable clock access to port A */
		RCC->AHBENR |= GPIOA_CLK_EN;

		/* Enable clock access to port C */
		RCC->AHBENR |= GPIOC_CLK_EN;

		/* Configure LED Pin as output pins */
		GPIOA->MODER |= USER_LED2_MODER;

		/* Configure P13 as input pins */
		GPIOC->MODER &=~(1U>>18);
		GPIOC->MODER &=~(1U>>19);


		while(1)
		{
			/* Turn on all LED (LD2) */
			// GPIOA->ODR ^= USER_LED2;

			/*
			 * Toggle LED (LD2) using XOR ^
			 */
			// GPIOA->ODR ^= USER_LED2;

			/*
			 * Toggle LED using BTN1
			 * LED stays on until button is pressed
			 */
			if (GPIOC->IDR & BTN_PIN)
			{
			GPIOA->BSRR ^= (1U<<5);
			}
			else
			{
			GPIOA->BSRR ^= (1U<<21);
			}
		}


}

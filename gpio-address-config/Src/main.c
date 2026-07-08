/*
 * I am using the NUCLEO-STM32F030R8 dev board
 *
 * Data sheet download:
 * https://www.st.com/en/evaluation-tools/nucleo-f030r8.html#documentation
 *
 * User Manual: https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf
 *
 * Reference Manual: https://www.st.com/resource/en/reference_manual/rm0360-stm32f030x4x6x8xc-and-stm32f070x6xb-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 *
 */

/*
 * Data detection, what ports and pins do I need to access?
 *
 * Goal 1: Manually toggle the user LED (LD2) w/out
 * using any libraries or pre-existing drivers
 *
 * Goal 2: Physically toggle LD2 with the user Button (BT1)
 *
 * Where is the LED (LD2) connected?
 * LEDs connect to found in the user guide:
 * Pin    (what pin(s))   5
 * Port   (what port(s))  A
 *
 * Where does Port A reside?
 * (Data Sheet, Memory Mapping Figure 10 pg. 38)
 *
 * Bus	  AHB1			  0x4002 0000
 * Bus    AHB2            0x4800 0000
 *
 * Where is the button connected?
 * Button (B1 USER):
 * Pin  13
 * Port C
 *
 */

/*
 * I want to access LD2 so I want to access the Peripherals Bus
 * (from the memory map diagram) beginning with APB 0x4000 0000=THIS IS PERIPH_BASE
 *
 * To gain control of LD2 I must access Port A which is located at GPIOA
(General Purpose Input Output A) on Bus AHB2 with a boundary address of
0x4800 0000 (AHB2_PERIPH_BASE) - 0x4800 03FF
 *
 * Find the AHB1_OFFSET and AHB1_PERIPH_BASE to access RCC (Reset Clock Controller)
 */

#define PERIPH_BASE			0x40000000UL

/* AHB1 to access RCC */
#define AHB1_OFFSET			0x00020000UL
#define AHB1_PERIPH_BASE	PERIPH_BASE + AHB1_OFFSET		// 0x4002 0000

/* AHB2 to access GPIOA */
#define AHB2_OFFSET			0x08000000UL
#define AHB2_PERIPH_BASE	PERIPH_BASE + AHB2_OFFSET		// 0x4800 0000
#define GPIOA_BASE			AHB2_PERIPH_BASE				// 0x4800 0000

/*
 * Issue: Incorrectly set RCC_OFFSET to 0x00021000UL
 *
 * Solution:  Because AHB1_PERIPH_BASE is equal to 0x40020000
 * 			  When adding to RCC_BASE while defining RCC_AHBENR
 * 			  originally equaled 40041000 (notice the rogue 4 on
 * 			  bit 4. I changed RCC_OFFSET to 0x1000UL allowing the
 * 			  2 on bit 4 to stay.
 */
#define RCC_OFFSET			0x1000UL
#define RCC_BASE			AHB1_PERIPH_BASE + RCC_OFFSET	// 0x4002 0000 + 0000 1000 = 0x4002 1000
#define RCC_AHBENR_OFFSET	0x14							// 0x0000 0014
/*
 * Type cast and dereference to a pointer
 * This creates the register
 */
#define RCC_AHBENR			(*(volatile unsigned int *)(RCC_BASE + RCC_AHBENR_OFFSET))

#define MODER_OFFSET		0x00UL
/*
 * Type cast and dereference to a pointer
 * This creates the register
 * I'm targeting MODER5 because LD2 is on Pin 5
 */
#define GPIOA_MODER			(*(volatile unsigned int *)(GPIOA_BASE + MODER_OFFSET))

#define ODR_OFFSET			0x14UL
#define GPIOA_ODR			(*(volatile unsigned int *)(GPIOA_BASE + ODR_OFFSET))


#define GPIOA_CLK_EN		(1U<<17)

#define USER_LED2_MODER		(1U<<10)

#define USER_LED2			(1U<<5)

/* Define my button (BT1) */
#define GPIOC_OFFSET		0x800UL
#define GPIOC_BASE			(AHB2_PERIPH_BASE + GPIOC_OFFSET)
#define GPIOC_MODER			(*(volatile unsigned int *)(GPIOC_BASE + MODER_OFFSET))
#define GPIOC_IDR_OFFSET	0x10UL
#define GPIOC_IDR			(*(volatile unsigned int *)(GPIOC_BASE + GPIOC_IDR_OFFSET))

#define GPIOC_CLK_EN		(1U<<19)

#define USER_BUTTON			(1U<<13)

/* Write the execution */

int main(void)
{
	/* Enable clock access to port A */
	RCC_AHBENR |= GPIOA_CLK_EN;

	/* Configure LED Pin as output pins */
	GPIOA_MODER |= USER_LED2_MODER;

	/* Configure button press */
	RCC_AHBENR |= GPIOC_CLK_EN;

	int button_prev = 1;

	/* Turn on LED using a while loop */
	while(1)
	{
		/* Turn on all LED (LD2) */
		// GPIOA_ODR |= USER_LED2;

		/* Toggle LED (LD2) using XOR ^
		 * I'm targeting bit 5 therefore
		 * XOR flips only bit 5 to produce
		 * the toggle effect
		 */
//		GPIOA_ODR ^= USER_LED2;
//		for(int i = 0; i < 1000000; i++){}

		/* Toggle LED with button press */
		int button_now = GPIOC_IDR & USER_BUTTON;

		if (button_now == 0 && button_prev != 0)
		{
			GPIOA_ODR ^= USER_LED2;
		}

		button_prev = button_now;



	}


}

/*
 * Definitions:
 * SB = solder bridge
 * Data Sheet    = features of the micro-chip
 * User Guide    = refers to the Nucleo board (not the chip)
 * Reference Man = refers to each register and each bit in
   	   	   	   	   the micro-chip
 */





























#include "../Inc/stm32f103x6.h"

void waitLoop(int loops);

int main(void)
{
	// Clock setup. External 8MHz.
	FLASH->ACR |= FLASH_ACR_LATENCY_2; //Set Flash Latency to 2 for 72MHz
	FLASH->ACR |= FLASH_ACR_PRFTBE; //Enable prefetch buffer
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; // HSE is on PD0 and PD1
	AFIO->MAPR &= ~(AFIO_MAPR_PD01_REMAP); // Set PD0 as OSC_In and PD1 as OSC_out
	RCC->CR |= RCC_CR_HSEON; // Turn in external clock input
	while(!((RCC->CR & RCC_CR_HSERDY)== RCC_CR_HSERDY)){;}
	RCC->CFGR |= RCC_CFGR_PLLMULL9; // Set PLL to x9
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // Set APB1 devider to /2
	RCC->CFGR |= RCC_CFGR_PLLSRC; // Set PREDIV1 as source for PLL
	RCC->CR |= RCC_CR_PLLON; // Turn on PLL
	RCC->CFGR |= RCC_CFGR_SW_PLL; // Select PLL for SYSCLOCK
	RCC->CFGR |= RCC_CFGR_MCO_SYSCLK; // Select sysclock for clock input
	RCC->CR &= ~(RCC_CR_HSION); // Turn off internal clock

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //Turn on Port B
	GPIOB->CRL |= GPIO_CRL_MODE7_1; 	//Set PB7 as output

	while(1)
	{
		GPIOB->BSRR |= GPIO_BSRR_BR7; //Turn off PB7
		waitLoop(0xFE);
		GPIOB->BSRR |= GPIO_BSRR_BS7; //Turn on PB7
		waitLoop(0xFE);
	}

	return 0;
}

void waitLoop(int loops)
{
	for(int i = 0; i < loops; i++)
	{
		for(int j = 0; j < 0xFE; j++)
		{
			;
		}
	}
}

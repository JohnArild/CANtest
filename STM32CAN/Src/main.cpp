/**
  ******************************************************************************
  * @file    main.cpp
  * @author  John Arild Lolland
  ******************************************************************************
*/

#include "BSP.h"

void waitLoop(int loops)
{
	for(int i = 0; i < loops; i++)
	{
		for(uint32_t j = 0; j < 0xFFF; j++)
		{
			;
		}
	}
}

int main(void)
{
	BSP_STM32CAN bsp;

	bsp.initilize();

	while(1)
	{
		GPIOA->BSRR |= GPIO_BSRR_BR_3;
		waitLoop(0xFE);
		GPIOA->BSRR |= GPIO_BSRR_BS_3;
		waitLoop(0xFE);
	}

	return 0;
}




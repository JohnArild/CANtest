/**
  ******************************************************************************
  * @file    BSP.cpp
  * @author  John Arild Lolland
  ******************************************************************************
*/

#include "BSP.h"

int BSP_STM32CAN::initilize (){
    BSP_STM32CAN::MCUspeed_48MHz();
	BSP_STM32CAN::setup_GPIO ();
	BSP_STM32CAN::setup_CAN ();
	BSP_STM32CAN::setup_SPI ();
    return 0;
}

int BSP_STM32CAN::MCUspeed_48MHz (){
	FLASH->ACR |= FLASH_ACR_LATENCY; // One wait-state for 48MHz operation
	FLASH->ACR |= FLASH_ACR_PRFTBE; // Prefetch buffer enable
	RCC->AHBENR |= RCC_AHBENR_GPIOFEN; // External crystal is on Port F
	RCC->CFGR |= RCC_CFGR_PLLMUL6; // Set PLL to x6 for 48MHz
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV; // HSE/PREDIV selected as PLL input clock
	RCC->CR |= RCC_CR_HSEON; // Turn in external clock input
	while(!((RCC->CR & RCC_CR_HSERDY)== RCC_CR_HSERDY)){;} // wait for HSE to be ready
	RCC->CR |= RCC_CR_PLLON;
	while(!((RCC->CR & RCC_CR_PLLRDY)== RCC_CR_PLLRDY)){;} // wait for PLL to be ready
	RCC->CFGR |= RCC_CFGR_SW_PLL; // Select PLL for SYSCLOCK
    return 0;
}

int BSP_STM32CAN::setup_GPIO (){
	//Enable ports
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	//Set up IO pins
	GPIOB->MODER |= GPIO_MODER_MODER1_0; // MCU_IO0 (output)
	GPIOA->MODER |= GPIO_MODER_MODER0_0; // MCU_IO1 (output)
	GPIOA->MODER |= GPIO_MODER_MODER1_0; // MCU_IO2 (output)
	GPIOA->MODER |= GPIO_MODER_MODER2_0; // MCU_IO3 (output)
	GPIOA->MODER |= GPIO_MODER_MODER3_0; // MCU_IO4 (output)
    return 0;
}

int BSP_STM32CAN::setup_CAN (){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_PA12_RMP; // Remap (pin pair PA11/12 mapped instead of PA9/10)
	GPIOA->MODER |= GPIO_MODER_MODER11_1; // CAN_RXD (alternative)
	GPIOA->MODER |= GPIO_MODER_MODER12_1; // CAN_TXD (alternative)
	GPIOA->AFR[1] |= 0x44000U; // Set Port 11 and 12 as AF4 for CAN
	//GPIOA->OTYPER |= GPIO_OTYPER_OT_11;
	//GPIOA->OTYPER |= GPIO_OTYPER_OT_12;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR11_0 |GPIO_OSPEEDR_OSPEEDR11_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR12_0 |GPIO_OSPEEDR_OSPEEDR12_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR11_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR12_0;
	RCC->APB1ENR |= RCC_APB1ENR_CANEN;
	CAN->MCR |= CAN_MCR_INRQ; // Initialization Request
	//CAN->BTR |= CAN_BTR_LBKM; // Debug mode
	//CAN->MCR |= CAN_MCR_NART; // No retransmitt

	//Create CAN message for testing:
	uint16_t CAN_ID = 0x123;
	uint32_t TIxR = 0;
	TIxR = (CAN_ID << 21); // move 11bit ID to bit 21-31
	while(!((CAN->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)){;} // Wait for INAK to become ready
	CAN->MCR &= ~(0x00010000); // Let CAN work in debug mode
	CAN->MCR &= ~(CAN_MCR_SLEEP); // Wake CAN
	CAN->MCR |= CAN_MCR_TXFP; // Transmit FIFO priority
	CAN->sTxMailBox[0].TDTR |= 0x4;
	CAN->sTxMailBox[0].TIR = TIxR;
	uint32_t CAN_Data = 0xDEADBEEF;
	CAN->sTxMailBox[0].TDLR = CAN_Data;
	CAN->MCR &= ~(CAN_MCR_INRQ);
	CAN->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
    return 0;
}

int BSP_STM32CAN::setup_SPI (){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER4_1; // SPI_NSS (alternative)
	GPIOA->MODER |= GPIO_MODER_MODER5_1; // SPI_SCK (alternative)
	GPIOA->MODER |= GPIO_MODER_MODER6_1; // SPI_MISO (alternative)
	GPIOA->MODER |= GPIO_MODER_MODER7_1; // SPI_MOSI (alternative)
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    return 0;
}

#include "ws2812.h"

#define 	LED_NUMS	4
#define 	LED_SIZE	2 * LED_NUMS

#define CODE1	0xf0
#define CODE0 0xc0

uint8_t LED_BUFFER[24*LED_SIZE + 10];

extern SPI_HandleTypeDef hspi1;

/**
	*	@brief	Transmit Data by DMA and SPI
	*	@param	pData:	pointer to Data Buffer(TX)
	*	@param	size:		amount of data to be sent
	*	@retval	None
	*/
void DMA_SPI_Transmit(uint8_t * pData, uint32_t size) 
{
	HAL_SPI_Transmit_DMA(&hspi1, pData, size);
}

/**
	*	@brief	Delay Control	
	*	@param	nCount:	number of time need to be delayed
	*	@retval	None
	*/
void delay_us(__IO uint16_t nCount) {
	__IO uint32_t time = nCount * (SystemCoreClock / 1000000);
	while (time --) {
		__nop();
	}
}

/**
	*	@brief	ws2812 driver
	*	@param	color:	 RGB array
	*	@param	len	:	amount of led need to be controlled
	*	@retval	None
	*/
void WS2812_sendbuf(uint8_t color[3], uint32_t size){
	uint16_t memaddr = __RGB2Code(color, size * 2);
	
	DMA_SPI_Transmit(LED_BUFFER, memaddr);
	delay_us(50);
}

/**
	*	@brief	RGB array transfer to Code array 
						RGB -> GRB 1 byte -> 8 byte
	*	@param	color:	RGB array
	*	@param	size:	amount of led need to be controlled
	*	@retval	length of array 
	*/
uint32_t __RGB2Code(uint8_t color[3], uint32_t size) {
	uint8_t i;
	uint32_t memaddr;
	memaddr = 0;
	while (size) {
		for (i = 0; i < 8; i++) { //green
			LED_BUFFER[memaddr] = ((color[1] << i) & 0x80) ? CODE1 : CODE0;
			memaddr ++;
		}
		for (i = 0; i < 8; i++) { //red
			LED_BUFFER[memaddr] = ((color[2] << i) & 0x80) ? CODE1 : CODE0;
			memaddr ++;
		}
		for (i = 0; i < 8; i++) { //blue
			LED_BUFFER[memaddr] = ((color[0] << i) & 0x80) ? CODE1 : CODE0;
			memaddr ++;
		}
		size--;
	}
	return memaddr;
}
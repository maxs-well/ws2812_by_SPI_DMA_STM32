
#ifndef __WS2812_H__
#define __WS2812_H__
#include "spi.h"

#ifdef __cplusplus
 extern "C" {
#endif
uint32_t __RGB2Code(uint8_t color[3], uint32_t size);
void WS2812_sendbuf(uint8_t color[3], uint32_t size);
void SPI_DMA_Transmit(uint8_t pData, uint32_t size);
void delay_us(__IO uint16_t nCount);

#ifdef __cplusplus
}
#endif
#endif
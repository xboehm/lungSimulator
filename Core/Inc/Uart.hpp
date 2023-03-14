#pragma once

#include "stm32l4xx_hal.h"
#include <span>
#include <cstddef>

class Uart{
public:
	Uart(UART_HandleTypeDef* handle, DMA_HandleTypeDef* dma);

	UART_HandleTypeDef* m_handle;
	void send(std::span<const std::byte> const buf) ;
	void send1(uint8_t* buf, uint16_t size);
	void sendChar(char buf);
	void receive(uint8_t* buf, uint16_t size);
	void receiveIT(uint8_t* buf, uint16_t size);
	void receiveToIdleDMA(uint8_t* buf, uint16_t size);

private:

	DMA_HandleTypeDef* m_dma;
};

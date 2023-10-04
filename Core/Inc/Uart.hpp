#pragma once

#include "stm32l4xx_hal.h"
#include <inttypes.h>
#include <span>
#include <string_view>
#include <cstddef>

class Uart{
public:
	Uart(UART_HandleTypeDef* handle, DMA_HandleTypeDef* dma);

	void send(std::span<const std::byte> const buf) ;
	void send(std::string_view word);
	void send(const char character);
	void send1(char* buf, uint16_t size);
	void sendChar(char buf);
	void receive(uint8_t* buf, uint16_t size);
	void receiveIT(uint8_t* buf, uint16_t size);
	void receiveToIdleDMA(uint8_t* buf, uint16_t size);
	void receiveDMA(uint8_t* buf, uint16_t size);
	void abortRx();


private:
	UART_HandleTypeDef* m_handle;
	DMA_HandleTypeDef* m_dma;
};

Uart& operator<< (Uart& out, const int number);
Uart& operator<< (Uart& out, const long unsigned int number);
Uart& operator<< (Uart& out, const float number);
Uart& operator<< (Uart& out, const char);
Uart& operator<< (Uart& out, std::span<const char> word);

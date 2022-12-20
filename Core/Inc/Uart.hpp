#pragma once

#include "stm32l4xx_hal.h"
#include <memory>
#include <span>
#include <cstddef>

class Uart{
public:
	Uart(UART_HandleTypeDef& handle);

	void send(std::span<const std::byte> const buf) ;
	void sendChar(char buf);

private:
	UART_HandleTypeDef& m_handle;
};

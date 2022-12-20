#pragma once

#include "Pinout.hpp"
#include "Uart.hpp"
#include "Adc.hpp"
#include "stm32l4xx_hal.h"
#include <array>
#include <charconv>

struct Application{
	Application(Pinout pinout, Uart uart, Adc adc);
	void loop();
private:
	Pinout m_pinout;
	Uart m_uart;
	Adc m_adc;
};

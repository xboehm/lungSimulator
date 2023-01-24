#pragma once

#include "Pinout.hpp"
#include "Uart.hpp"
#include "Adc.hpp"
#include "MD10C.hpp"
#include "stm32l4xx_hal.h"
#include <array>
#include <charconv>

struct Application{
	Application(UART_HandleTypeDef*  uart, ADC_HandleTypeDef* adc,
							TIM_HandleTypeDef* TIMhandle, TIM_TypeDef* timer, uint32_t channel);
	void loop();
private:
	Pinout m_pinout;
	Uart m_uart;
	Adc m_adc;
	MD10C m_motor;
};

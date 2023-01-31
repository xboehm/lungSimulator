#pragma once

#include "Pinout.hpp"
#include "Uart.hpp"
#include "Adc.hpp"
#include "MD10C.hpp"
#include "stm32l4xx_hal.h"
#include <array>
#include <charconv>

class Application{
public:
	static Application& getInstance(UART_HandleTypeDef*  uart, ADC_HandleTypeDef* adc,
			TIM_HandleTypeDef* TIMhandle);
	Application(const Application&) = delete;	//delete copy constructor
	void loop();


private:
	Application(UART_HandleTypeDef*  uart, ADC_HandleTypeDef* adc,
								TIM_HandleTypeDef* TIMhandle);

	Pinout m_pinout;
	Uart m_uart;
	Adc m_adc;
	MD10C m_motor;

	enum class State{
		init,
		menu,
		breathe,
		stop
	};
};

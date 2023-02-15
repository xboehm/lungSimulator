#pragma once

#include "Pinout.hpp"
#include "Uart.hpp"
#include "Adc.hpp"
#include "MD10C.hpp"
#include "Cli.hpp"
#include "stm32l4xx_hal.h"
#include <array>
#include <charconv>
#include <string_view>
#include <algorithm>

class Cli;
class Application{
public:
	static Application& getInstance(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma,
			ADC_HandleTypeDef* adc, TIM_HandleTypeDef* TIMhandle);
	Application(const Application&) = delete;	//delete copy constructor
	void loop();
	void m_printADCloop(int delay);
	void m_readADCbuffer();
	void m_buttonTest();

	friend class Cli;
	enum class State{
		init,
		menu,
		breathe,
		stop
	};

private:
	Application(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc,
								TIM_HandleTypeDef* TIMhandle);



	Pinout m_pinout;
	Uart m_uart;
	Adc m_adc;
	MD10C m_motor;
	State m_currentState {State::init};

public:
	bool m_endFlag {false};
	//uint32_t m_adcRaw {0};
	bool m_adcComplete {false};
	bool m_uartComplete {false};
};

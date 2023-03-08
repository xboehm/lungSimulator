#pragma once

#include "Pinout.hpp"
#include "Uart.hpp"
#include "Adc.hpp"
#include "MD10C.hpp"
#include "stm32l4xx_hal.h"
#include <array>
#include <charconv>
#include <string_view>
#include <algorithm>

class Application{
public:
	static Application& getInstance(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma,
			ADC_HandleTypeDef* adc, TIM_HandleTypeDef* TIMhandle);
	Application(const Application&) = delete;	//delete copy constructor
	void loop();
	std::string_view m_printState() const;
	void m_printADCloop(int delay);
	void m_readADCbuffer();
	void m_buttonTest();

	void CLIversion();
	void CLIblink();
	void CLIbreathe();
	void CLIpause();
	void CLIfeed();

	enum class State{
		init,
		menu,
		feed,
		breathe,
		stop,
	};

private:
	Application(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc,
						 TIM_HandleTypeDef* TIMhandle);
	Pinout m_pinout;
	Uart m_uart;
	Adc m_adc;
	MD10C m_motor;
	State m_currentState {State::init};
	float calcPosition(uint16_t adc);

public:
	bool m_endFlag {false};
	bool m_adcComplete {false};
	bool m_uartComplete {false};
	bool m_regTimer {false};
	uint16_t m_uartSize {0};
};

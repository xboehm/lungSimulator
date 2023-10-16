#pragma once

#include "CustomTypes.hpp"
#include "Pinout.hpp"
#include "Uart.hpp"
#include "Adc.hpp"
#include "MD10C.hpp"
#include "Breath.hpp"
#include "stm32l4xx_hal.h"
#include <array>
#include <charconv>
#include <string_view>


class Application{
public:
	static Application& getInstance(DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc,
															TIM_HandleTypeDef* TIMhandle, UART_HandleTypeDef*  uart);
	Application(const Application&) = delete;	//delete copy constructor
	void loop();
	std::string_view m_printState() const;
	void m_printADCloop(int delay);
	void m_readADCbuffer();
	void m_buttonTest();
	void m_directionTest();
	//good values are 600, 1200U, 3000U
	void m_moveEndToEnd(int speed, unsigned int lowerEnd, unsigned int upperEnd);
	void m_stopMotor();
	void m_getEndPositions();
	void m_toggleTimerPin();
	void m_copyPattern(std::span<const float> data);

	void CLIversion();
	void CLIblink();
	void CLIbreathe();
	void CLIselect(CommandPayload& payload);
	void CLIpause();
	void CLIfreq(CommandPayload& payload);
	void CLIvol(CommandPayload& payload);
	void CLIchange(CommandPayload& payload);
	void CLIfeed();

	enum class State{
		init,
		menu,
		breathe,
		stop
	};

private:
	Application(DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc,
						 TIM_HandleTypeDef* TIMhandle, UART_HandleTypeDef*  uart);
	Pinout m_pinout;
	Adc m_adc;
	MD10C m_motor;
	State m_currentState {State::init};
	BreathingPattern m_breathingPattern {};
	BreathingPattern m_inputPattern {};
	int m_breathCounter {0};
	bool m_paramChange {false};
	float m_requestedFreq {0};
	float m_step {1.0f};
	int m_endTime {0};
	int m_requestedVolume {0};
	float m_volFactor {1};
	//converts volume into position
	float m_positionFactor {0.1989437f};
	int m_cylVolume {800};
	float m_calcPosition(uint16_t adc);

public:
	volatile bool m_endFlag {false};
	volatile bool m_adcComplete {false};
	volatile bool m_uartComplete {false};
	volatile bool m_regTimer {false};
	volatile uint16_t m_uartSize {0};
	volatile bool m_process {false};
	Uart m_uart;
	std::array<uint8_t, 7007> patternInput {};
	uint8_t* index {patternInput.begin()};
	bool m_inpAvail {false};
	bool m_diffPattern {false};
};

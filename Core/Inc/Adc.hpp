#pragma once

#include "GlobalConf.hpp"
#include "stm32l4xx_hal.h"
#include <array>
#include <memory>

class Adc{
public:
	Adc(ADC_HandleTypeDef* handle);
	uint32_t readSinglePoll();
	uint32_t readValue();
	void startConversionInterrupt();
	void readSingleLoop(unsigned int delay, std::array<uint32_t, constants::ADCreads>& buffer);

private:
	ADC_HandleTypeDef* m_handle;
};

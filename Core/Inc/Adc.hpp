#pragma once

#include "GlobalConf.hpp"
#include "stm32l4xx_hal.h"
#include <array>
#include <memory>

using readBuffer = std::array<uint32_t, constants::ADCreads>;

class Adc{
public:
	Adc(ADC_HandleTypeDef* handle);
	uint32_t readSingle();
	void readSingleLoop(unsigned int delay, readBuffer& buffer);

private:
	ADC_HandleTypeDef* m_handle;
};

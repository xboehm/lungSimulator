#pragma once

#include "stm32l4xx_hal.h"
#include <memory>

class PWM{
public:
	PWM(TIM_HandleTypeDef* handle, TIM_TypeDef* timer, uint32_t channel);
	void start();
	void stop();
	void changePulse(uint16_t pulse);
private:
	TIM_HandleTypeDef* m_handle;
	TIM_TypeDef* m_timer;
	uint32_t m_channel;
};

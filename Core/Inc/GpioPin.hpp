#pragma once

#include "stm32l4xx_hal.h"

class GpioPin{
public:
	GpioPin(GPIO_TypeDef* port, uint16_t pin);

	bool read();
	void set();
	void clear();
	void toggle();
	//void delay(int milliseconds);

private:
	GPIO_TypeDef* m_GpioPort;
	uint16_t m_GpioPin {};
};

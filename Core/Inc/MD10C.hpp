#pragma once
#include "Pinout.hpp"
#include "PWM.hpp"

class MD10C{
public:
	MD10C(GpioPin& dir, TIM_HandleTypeDef* TIMhandle, TIM_TypeDef* timer, uint32_t channel);
	void forward();
	void reverse();
	void stop();
	void selfTest();

private:
	GpioPin& m_dir;
	PWM m_pwm;
	bool m_pwmStarted {false};

	void startPWM(bool started);
};

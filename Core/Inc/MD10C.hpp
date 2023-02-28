#pragma once
#include "Pinout.hpp"
#include "PWM.hpp"

class MD10C{
public:
	MD10C(GpioPin& dir, TIM_HandleTypeDef* TIMhandle);
	void forward();
	void reverse();
	void stop();
	void selfTest();
	void setSpeed(uint16_t speed);

private:
	GpioPin& m_dir;
	PWM m_pwm;
	bool m_pwmStarted {false};

	void startPWM(bool started);
};

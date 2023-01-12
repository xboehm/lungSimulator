#pragma once
#include "Pinout.hpp"
#include "PWM.hpp"

class MD10C{
public:
	MD10C(GpioPin& dir, PWM& pwm);
	void forward();
	void reverse();
	void stop();

private:
	GpioPin& m_dir;
	PWM& m_pwm;
	bool m_pwmStarted {false};

	void startPWM(bool started);
};

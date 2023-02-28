#include "MD10C.hpp"

MD10C::MD10C(GpioPin& dir, TIM_HandleTypeDef* TIMhandle)
	: m_dir {dir}, m_pwm {TIMhandle, TIM1, TIM_CHANNEL_1}
{
}

void MD10C::forward() {
	m_dir.clear();
	startPWM(m_pwmStarted);
}

void MD10C::reverse() {
	m_dir.set();
	startPWM(m_pwmStarted);
}

void MD10C::stop() {
	m_pwm.stop();
	m_pwmStarted = false;
}

void MD10C::startPWM(bool started) {
	if(!started){
			m_pwm.start();
			m_pwmStarted = true;
	}
}

void MD10C::selfTest() {
	forward();
	HAL_Delay(600);
	stop();
	HAL_Delay(1000);
	reverse();
	HAL_Delay(600);
	stop();
}

void MD10C::setSpeed(uint16_t speed) {
	m_pwm.changePulse(speed);
}

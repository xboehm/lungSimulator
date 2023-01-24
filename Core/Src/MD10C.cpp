#include "MD10C.hpp"

MD10C::MD10C(GpioPin& dir, TIM_HandleTypeDef* TIMhandle)
	: m_dir(dir), m_pwm (TIMhandle, TIM1, TIM_CHANNEL_1)
{
}

void MD10C::forward(){
	startPWM(m_pwmStarted);
	m_dir.clear();
}

void MD10C::reverse(){
	startPWM(m_pwmStarted);
	m_dir.set();
}

void MD10C::stop(){
	//set PWM low, turn it off, stop timer
	m_pwm.stop();
	m_pwmStarted = false;
}

void MD10C::startPWM(bool started){
	if(!started){
			m_pwm.start();
			m_pwmStarted = true;
	}
}

void MD10C::selfTest(){
	forward();
	 HAL_Delay(600);
	 stop();
	 HAL_Delay(1000);
	 reverse();
	 HAL_Delay(600);
	 stop();
}

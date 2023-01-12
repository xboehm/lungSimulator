#include "MD10C.hpp"

MD10C::MD10C(GpioPin& dir, PWM& pwm)
	: m_dir(dir), m_pwm(pwm)
{}

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

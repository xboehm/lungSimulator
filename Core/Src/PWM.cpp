#include "PWM.hpp"

PWM::PWM(TIM_HandleTypeDef* handle, TIM_TypeDef* timer, uint32_t channel)
	: m_handle {handle}, m_timer {timer}, m_channel {channel}
{}

void PWM::start(){
	HAL_TIM_PWM_Start(m_handle, m_channel);
}

void PWM::stop(){
	HAL_TIM_PWM_Stop(m_handle, m_channel);
}

void PWM::changeDutyCycle(uint16_t dutyCycle){
	//if requested duty cycle is less than or equal to 100%
	if(dutyCycle <= (m_timer->ARR)){
		m_timer->CCR1 = dutyCycle;
	}
	else{
		//Error handling
		//requested pulse out of range, max = m_timer->ARR
	}
}

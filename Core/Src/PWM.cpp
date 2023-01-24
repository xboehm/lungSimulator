#include "PWM.hpp"

PWM::PWM(TIM_HandleTypeDef* handle, TIM_TypeDef* timer, uint32_t channel)
	: m_handle(handle), m_timer(timer), m_channel(channel)
{}

void PWM::start(){
	HAL_TIM_PWM_Start(m_handle, m_channel);
}

void PWM::stop(){
	HAL_TIM_PWM_Stop(m_handle, m_channel);
}

void PWM::changePulse(unsigned int pulse){
	if(pulse < (m_timer->ARR+1)){
		m_timer->CCR1 = pulse;
	}
	else{
		//Error handling
	}
}

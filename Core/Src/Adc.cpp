#include "Adc.hpp"

Adc::Adc(ADC_HandleTypeDef* handle)
	: m_handle {handle}
{
}

uint32_t Adc::readSinglePoll(){
	HAL_ADC_Start(m_handle);
	if(HAL_ADC_PollForConversion(m_handle, 10) == HAL_OK) {
		return HAL_ADC_GetValue(m_handle);
	}
	else{
		//Error handling
		return 0xFFFFFFFF;
	}
}

//iterator, for each loop, for_each algorithm with lambda? what offers the most performance?
void Adc::readSingleLoop(unsigned int delay, readBuffer& buffer){
	for(auto& value: buffer){
		HAL_ADC_Start(m_handle);
		HAL_ADC_PollForConversion(m_handle, HAL_MAX_DELAY);
		value = HAL_ADC_GetValue(m_handle);
		HAL_Delay(delay);
	}
}

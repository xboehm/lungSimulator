#include "Adc.hpp"

Adc::Adc(ADC_HandleTypeDef& handle)
	: m_handle (handle)
{
}

uint32_t Adc::readSingle(){
	HAL_ADC_Start(std::addressof(m_handle));
	HAL_ADC_PollForConversion(std::addressof(m_handle), HAL_MAX_DELAY);
	return HAL_ADC_GetValue(std::addressof(m_handle));
}

//iterator, for each loop, for_each algorithm with lambda? what offers the most performance?
void Adc::readSingleLoop(unsigned int delay, readBuffer& buffer){
	for(auto& value: buffer){
		HAL_ADC_Start(std::addressof(m_handle));
		HAL_ADC_PollForConversion(std::addressof(m_handle), HAL_MAX_DELAY);
		value = HAL_ADC_GetValue(std::addressof(m_handle));
		HAL_Delay(delay);
	}
}

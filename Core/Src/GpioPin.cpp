#include "GpioPin.hpp"
#include <memory>

GpioPin::GpioPin(GPIO_TypeDef* port, uint16_t pin)
	: m_GpioPort {port}, m_GpioPin{pin}
{
}

bool GpioPin::read(){
	return HAL_GPIO_ReadPin(m_GpioPort, m_GpioPin);
}

void GpioPin::set(){
	HAL_GPIO_WritePin(m_GpioPort, m_GpioPin, GPIO_PIN_SET);
}

void GpioPin::clear(){
	HAL_GPIO_WritePin(m_GpioPort, m_GpioPin, GPIO_PIN_RESET);
}

void GpioPin::toggle(){
	HAL_GPIO_TogglePin(m_GpioPort, m_GpioPin);
}

//void GpioPin::delay(int milliseconds){
//	HAL_Delay(milliseconds);
//}

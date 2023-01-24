#include "Application.hpp"

Application::Application(UART_HandleTypeDef*  uart, ADC_HandleTypeDef* adc,
		TIM_HandleTypeDef* TIMhandle, TIM_TypeDef* timer, uint32_t channel)
	: m_uart (uart), m_adc (adc), m_motor (m_pinout.m_dir, TIMhandle, timer, channel)
{
}

Application& Application::getInstance(UART_HandleTypeDef*  uart, ADC_HandleTypeDef* adc,
		TIM_HandleTypeDef* TIMhandle, TIM_TypeDef* timer, uint32_t channel){
	static Application m_instance {uart, adc, TIMhandle, timer, channel};
	return m_instance;
}

void Application::loop(){
	/*readBuffer buffer {};
	m_adc.readSingleLoop(2U, buffer);
	std::array<char, 8> test {"Start\r\n"};
	m_uart.send(std::as_bytes(std::span{test}));
	std::array<char, 6> output {};
	for(auto element: buffer){
		std::to_chars(output.data(), output.data()+output.size(), element);
		output.at(4) = '\r';
		output.at(5) = '\n';
		m_uart.send(std::as_bytes(std::span{output}));
		output.fill(0);
	}*/
	m_motor.selfTest();
}

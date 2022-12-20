#include "Application.hpp"

Application::Application(Pinout pinout, Uart uart, Adc adc)
	: m_pinout (pinout), m_uart (uart), m_adc (adc)
{
}

void Application::loop(){
	readBuffer buffer {};
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
	}

}

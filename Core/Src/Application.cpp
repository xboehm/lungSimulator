#include "Application.hpp"

Application::Application(UART_HandleTypeDef*  uart, ADC_HandleTypeDef* adc,
													TIM_HandleTypeDef* TIMhandle)
	:m_pinout {}, m_uart {uart}, m_adc {adc}, m_motor {m_pinout.m_dir, TIMhandle}
{
}

Application& Application::getInstance(UART_HandleTypeDef*  uart, ADC_HandleTypeDef* adc,
		TIM_HandleTypeDef* TIMhandle)
{
	static Application m_instance {uart, adc, TIMhandle};
	return m_instance;
}

void Application::loop() {
	uint32_t adcRaw {m_adc.readSinglePoll()};

	while(1) {
		switch(m_currentState){
			case(State::init):
				//do initialization
				m_motor.reverse();
				m_currentState = State::breathe;
/*
				if(init.done()){
					//critical section start
					irq.disable();
					currentGlobalState = State::menu;
					irq.enable();
					//critical section end
				}
*/
				break;

			case(State::menu):
					break;

			case(State::breathe):
				if(m_endFlag) {
					m_currentState = State::stop;
					break;
				}
				//do regulation
				adcRaw = m_adc.readSinglePoll();
				if(adcRaw < 1200U){
					m_motor.forward();
				}
				else if(adcRaw > 2800U){
					m_motor.reverse();
				}
				break;

			case(State::stop):
				m_motor.stop();
				//wait for system being cleared
				while(1) {
					m_pinout.m_onboardLed.toggle();
					//blue button pressed && end switch released
					//bool button = m_pinout.m_blueButton.read();

					if(m_pinout.m_blueButton.read() && m_pinout.m_endR.read()) {
						//critical section start
						//irq.disable();
						m_endFlag = false;
						//irq.enable();
						//critical section end
						m_currentState = State::init;
						break;
					}

					HAL_Delay(50);
				}
		}
	}

}

void Application::m_printADCloop(int delay) {
	uint32_t raw {};
	std::array<char, 6> output {};
	while(1) {
		raw = m_adc.readSinglePoll();
		std::to_chars(output.data(), output.data()+output.size(), raw);
		output.at(4) = '\r';
		output.at(5) = '\n';
		m_uart.send(std::as_bytes(std::span{output}));
		output.fill(0);
		HAL_Delay(delay);
	}
}

void Application::m_readADCbuffer() {
	std::array<uint32_t, constants::ADCreads> buffer {};
	m_adc.readSingleLoop(2U, buffer);
	std::array<char, 6> output {};
	for(auto element: buffer){
		std::to_chars(output.data(), output.data()+output.size(), element);
		output.at(4) = '\r';
		output.at(5) = '\n';
		m_uart.send(std::as_bytes(std::span{output}));
		output.fill(0);
	}
}

void Application::m_buttonTest(){
	int blue {};
	int end {};
	std::array<char, 5> sendBuf;
	while(1) {
		blue = m_pinout.m_blueButton.read();
		end = m_pinout.m_endR.read();
		std::to_chars(sendBuf.data(), sendBuf.data()+1, blue);
		std::to_chars(sendBuf.data()+1, sendBuf.data()+2, end);
		sendBuf.at(3) = '\r';
		sendBuf.at(4) = '\n';
		m_uart.send(std::as_bytes(std::span{sendBuf}));
		sendBuf.fill(0);
		HAL_Delay(500);
	}
}

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

	State currentGlobalState {State::breathe};
	uint32_t adcRaw {m_adc.readSinglePoll()};


	while(1) {
		switch(currentGlobalState){
			case(State::init):
				//do initialization
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
				//do regulation
				m_motor.forward();
				while(adcRaw > 1200){
					adcRaw = m_adc.readSinglePoll();
				}
				m_motor.reverse();
				while(adcRaw < 2800){
					adcRaw = m_adc.readSinglePoll();
				}
				break;
//				if(adcRaw < 2048){
//					m_motor.reverse();
//				}
//				else{
//					m_motor.forward();
//				}

			case(State::stop):
				//wait for system being cleared
				/*
				if(releaseButton.pressed() && endschalter.released()){
					//critical section start
					irq.disable();
					currentGlobalState = State::init;
					irq.enable();
					//critical section end
					 */
				 break;
		}
	}
}

#include "Application.hpp"

Application::Application(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc,
													TIM_HandleTypeDef* TIMhandle)
	:m_pinout {}, m_uart {uart, dma}, m_adc {adc}, m_motor {m_pinout.m_dir, TIMhandle}
{
}

Application& Application::getInstance(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma,
		ADC_HandleTypeDef* adc, TIM_HandleTypeDef* TIMhandle)
{
	static Application m_instance {uart, dma, adc, TIMhandle};
	return m_instance;
}

void Application::loop() {
	Cli cli {};
	bool found {false};
	uint8_t rxBuf[15] {};
	char readBuf[15] {};
	uint32_t adcRaw {m_adc.readSinglePoll()};
	while(1) {
		switch(m_currentState){
			case(State::init):
				//do initialization
				//m_motor.reverse();
				//m_currentState = State::breathe;

/*
				if(init.done()){
					//critical section start
					irq.disable();
					currentGlobalState = State::menu;
					irq.enable();
					//critical section end
				}
*/
				m_currentState = State::menu;
				break;

			case(State::menu):
			{
				m_uart.send(std::as_bytes(std::span{"<Enter command> "}));
				m_uart.receiveToIdleDMA(rxBuf, 15);
				while(!m_uartComplete){
					HAL_Delay(10);
				}
				//clear isr flag
				m_uartComplete = false;
				//copy rxBuf to readBuf to ensure that input string_view views a string that won't change
				std::copy(rxBuf, rxBuf+std::size(rxBuf), readBuf);
				std::string_view input {(const char*)readBuf};
				//'help macro' prints all available commands with their description
				if(input == "help"){
					found = true;
					m_uart.send(std::as_bytes(std::span{"\r\nAvailable commands:\r\n"}));
					for(auto& cmd: cli.commands) {
						m_uart.send(std::as_bytes(std::span{"Name: "}));
						m_uart.send(std::as_bytes(std::span{cmd.name}));
						m_uart.send(std::as_bytes(std::span{" ("}));
						m_uart.send(std::as_bytes(std::span{cmd.help}));
						m_uart.send(std::as_bytes(std::span{")\r\n"}));
					}
				}
				else{
					//check if the input matches a known command name. If so, execute it
					for(auto& element: cli.commands){
						if(input == element.name){
							found = true;
							element.execute(this);
							break;
						}
					}
				}
				if(!found) {
					m_uart.send(std::as_bytes(std::span{"Command not found.\r\n"}));
				}
				else {
					found = false;
				}
				//clear rxBuf
				for(std::size_t i{0}; i<std::size(rxBuf); ++i){
					rxBuf[i] = '\0';
				}
				break;
				}

			case(State::breathe):
				m_adc.startConversionInterrupt();
				if(m_endFlag) {
					m_currentState = State::stop;
					break;
				}
				//do regulation
				if(m_adcComplete) {
					adcRaw = m_adc.readValue();
					m_adcComplete = false;
					if(adcRaw < 1200U){
						m_motor.forward();
					}
					else if(adcRaw > 2800U){
						m_motor.reverse();
					}
				}
				break;

			case(State::stop):
				m_motor.stop();
				//wait for system being cleared
				while(1) {
					m_pinout.m_onboardLed.toggle();
					//blue button pressed && end switch released
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

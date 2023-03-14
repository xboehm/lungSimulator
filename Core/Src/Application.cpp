#include "Application.hpp"
#include "Cli.hpp"
#include "Command.hpp"
#include "sine.hpp"
#include "Pid.hpp"
#include "pattern1.hpp"
#include <cstdio>

Application::Application(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc,
													TIM_HandleTypeDef* TIMhandle)
	:m_pinout {}, m_uart {uart, dma}, m_adc {adc}, m_motor {m_pinout.m_dir, TIMhandle}  {}

Application& Application::getInstance(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma,
		ADC_HandleTypeDef* adc, TIM_HandleTypeDef* TIMhandle)
{
	static Application m_instance {uart, dma, adc, TIMhandle};
	return m_instance;
}


extern Application& application;
void Application::loop() {
	CmdVersion cmdversion{application};
	CmdBlink cmdblink{application};
	CmdBreathe cmdbreathe{application};
	CmdPause cmdpause{application};
	CmdEndpos cmdendpos{application};
	std::array commands{
		static_cast<Command*>(&cmdversion),
		static_cast<Command*>(&cmdblink),
		static_cast<Command*>(&cmdbreathe),
		static_cast<Command*>(&cmdpause),
		static_cast<Command*>(&cmdendpos),
	};
	Cli cli {commands, m_uart, m_uartSize, application};
	bool menuFirstEntry {true};
	PID pid {240.0f, 50.0f, 0.001f, 0.002f, -999, 999, -300.0f, 300.0f, 0.001f};
	std::size_t sample {0};
	std::size_t maxSample {pattern::sine.size()-1};
	int speed {0};
	//time in ms
	unsigned short time {0};
	float position {0.0f};
	char buf[20] {};
	int breathCounter {0};
	while(1) {
		switch(m_currentState){
			case(State::init):
	{
				//do initialization
					//direction Test
//					m_motor.forward();
//					m_motor.setSpeed(800);
//					HAL_Delay(400);
//					m_motor.reverse();
//					HAL_Delay(400);
//					m_motor.stop();

					menuFirstEntry = true;


/*
				if(init.done()){
					//critical section start
					irq.disable();
					currentGlobalState = State::menu;
					irq.enable();
					//critical section end
				}
*/

//				sample = 120;
//				position = 421.54f;
//				speed = -777;
//				char buffer [20] {};
//				m_pinout.m_timerPin.set();
//				std::snprintf(&buffer[0], 20, "%6.2f,%6.2f,%+4i\n", pattern::sine[sample].position , position, speed);
//				std::snprintf(&buffer[0], 19, "567.45,-913\n");
//				HAL_UART_Transmit_DMA(m_uart.m_handle, (uint8_t*)&buffer[0], 19);
//				m_pinout.m_timerPin.set();
//				HAL_StatusTypeDef halStatus = HAL_UART_Transmit_DMA(m_uart.m_handle, (uint8_t*)&buffer[0], 12);
//				for(unsigned int i{0}; i < 14; ++i){
//					buffer[i] = '\0';
//				}
//				m_pinout.m_timerPin.clear();
//				sample = 0;
//				position = 0.0f;
//				speed = 0;
//
//				while(1){}

//				std::snprintf(buf, std::size(buf)-1, "%f", flo);
//				m_uart.send(std::as_bytes(std::span{buf}));
				m_currentState = State::menu;
				break;
	}

			case(State::menu):
				if(menuFirstEntry){
					cli.listen();
					menuFirstEntry = false;
				}
				HAL_Delay(20);
				//new command?
				if(m_uartComplete) {
					cli.decode();
					//clear isr flag
					m_uartComplete = false;
				}
				break;


			case(State::breathe):
				//check for end detection
				if(m_endFlag) {
//					m_motor.stop();
					m_currentState = State::stop;
					break;
				}
				//do regulation
				if(m_regTimer) {
					m_pinout.m_timerPin.set();
					m_adc.startConversionInterrupt();
					//wait for new ADC value
					while(!m_adcComplete) {
						if(m_endFlag) {
//							m_motor.stop();
							m_currentState = State::stop;
							break;
						}
					}
					//calculate new position measurement in mm
					position = m_calcPosition(m_adc.readValue());
					m_adcComplete = false;
					//get correct sample
					if(time>pattern::sine[sample].time) {
						++sample;
					}
					//calculate control update
					speed = pid.update(pattern::sine[sample].position, position);

					//direction logic
					if(speed < 0) {
						m_motor.reverse();
//						speed = -speed;
						m_motor.setSpeed(-speed);
					}
					else{
						m_motor.forward();
						m_motor.setSpeed(speed);
					}


					//LOG DATA HERE
					if(breathCounter < 3) {
						std::snprintf(buf, 20, "%6.2f,%6.2f,%+4i\n", pattern::sine[sample].position, position, speed);
//						if(buflength > 0) {
//							m_uart.send(std::as_bytes(std::span{buf, static_cast<unsigned int>(buflength)}));
							HAL_UART_Transmit(m_uart.m_handle, (uint8_t*)&buf[0], 19, HAL_MAX_DELAY);
//						}
//						for(unsigned int i{0}; i < std::size(buf); ++i){
//							buf[i] = '\0';
//						}
					}

					//logic to start pattern from the beginning if end was reached
					if(time==pattern::sine[maxSample].time){
						time = 1;
						sample = 1;
						++breathCounter;
					}
					else{
						++time;
					}
					m_regTimer = false;

					m_pinout.m_timerPin.clear();
				}

			/* moving from end to end */
//				m_adc.startConversionInterrupt();
//				if(m_adcComplete) {
//					adcRaw = m_adc.readValue();
//					m_adcComplete = false;
//					if(adcRaw < 1200U){
//						m_motor.forward();
//					}
//					else if(adcRaw > 3000U){
//						m_motor.reverse();
//					}
//				}

				//new command?
				if(m_uartComplete) {
					cli.decode();
					//clear isr flag
					m_uartComplete = false;
				}
				break;

			case(State::stop):
//				m_motor.stop();
				//wait for system being cleared
				while(1) {
					m_pinout.m_onboardLed.toggle();
					//blue button pressed and both end switches released
					if(m_pinout.m_blueButton.read() && m_pinout.m_endC.read() && m_pinout.m_endO.read()) {
						//critical section start
						//irq.disable();
						m_endFlag = false;
						//irq.enable();
						//critical section end
						m_pinout.m_onboardLed.clear();
						m_currentState = State::init;
						break;
					}

					HAL_Delay(50);
				}
		}
	}

}

std::string_view Application::m_printState() const {
	switch(m_currentState){
	case State::init:
		return "(init)";
	case State::menu:
		return "(menu)";
	case State::breathe:
		return "(breathe)";
	case State::stop:
		return "(stop)";
	default:
		return "(unknown state)";
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
	std::array<char, 5> sendBuf {};
	while(1) {
		blue = m_pinout.m_blueButton.read();
		end = m_pinout.m_endC.read();
		std::to_chars(sendBuf.data(), sendBuf.data()+1, blue);
		std::to_chars(sendBuf.data()+1, sendBuf.data()+2, end);
		sendBuf.at(3) = '\r';
		sendBuf.at(4) = '\n';
		m_uart.send(std::as_bytes(std::span{sendBuf}));
		sendBuf.fill(0);
		HAL_Delay(500);
	}
}

//at the right end of cylinder function returns 10mm
float Application::m_calcPosition(uint16_t adc) {
	return adc*constants::lsbLength-13.23737f;
}

void Application::m_stopMotor() {
	m_motor.stop();
}

void Application::m_getEndPositions() {
	m_motor.reverse();
	m_motor.setSpeed(420);
	//wait for end detection;
	while(!m_endFlag){
		//no optimization!
	}
//	m_motor.stop();
	uint32_t endPosClosed {m_adc.readSinglePoll()};
	HAL_Delay(10);
	m_motor.forward();
	HAL_Delay(200);
	m_endFlag = false;
	//wait for end detection;
	while(!m_endFlag){
		//no optimization!
	}
//	m_motor.stop();
	uint32_t endPosOpen {m_adc.readSinglePoll()};
	HAL_Delay(10);
	m_motor.reverse();
	HAL_Delay(300);
	m_motor.stop();
	m_endFlag = false;
	char buf[12] {};
	auto buflength {std::snprintf(buf, std::size(buf), "%lu, %lu\n", endPosClosed, endPosOpen)};
	if(buflength > 0) {
		m_uart.send(std::as_bytes(std::span{"Endpositions: "}));
		m_uart.send(std::as_bytes(std::span{buf, static_cast<unsigned int>(buflength)}));
	}
}

void Application::m_toggleTimerPin() {
	m_pinout.m_timerPin.clear();
}

void Application::CLIversion()  {
	m_uart.send(std::as_bytes(std::span{"Version: 0.1\r\n"}));
}

void Application::CLIblink()  {
	while(1){
		m_pinout.m_onboardLed.toggle();
		HAL_Delay(500);
	}
}

void Application::CLIbreathe() {
	m_currentState = State::breathe;
}

void Application::CLIpause() {
	m_motor.stop();
	m_currentState = State::menu;
	//menu first entry = true;
}

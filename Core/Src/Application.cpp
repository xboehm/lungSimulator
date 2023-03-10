#include "Application.hpp"
#include "Cli.hpp"
#include "Command.hpp"
#include "sine.hpp"
#include "Pid.hpp"
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
	std::array commands{
		static_cast<Command*>(&cmdversion),
		static_cast<Command*>(&cmdblink),
		static_cast<Command*>(&cmdbreathe),
		static_cast<Command*>(&cmdpause),
	};
	Cli cli {commands, m_uart, m_uartSize, application};
	bool menuFirstEntry {true};
	uint32_t adcRaw {m_adc.readSinglePoll()};
	PID pid {200.0f, 20.0f, 0.005f, 0.005f, -999, 999, -300.0f, 300.0f, 0.001f};
	std::size_t sample {0};
	std::size_t maxSample {pattern::sinePattern.size()-1};
	int speed {0};
	//time in ms
	unsigned short time {0};
	float position {0.0f};
	char buf[10];
	bool firstBreath {true};
	bool temptemp {true};
	while(1) {
		switch(m_currentState){
			case(State::init):
				//do initialization
					//direction Test
//					m_motor.forward();
//					m_motor.setSpeed(800);
//					HAL_Delay(400);
//					m_motor.reverse();
//					HAL_Delay(400);
//					m_motor.stop();

					firstBreath = true;
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
//				std::snprintf(buf, std::size(buf)-1, "%f", flo);
//				m_uart.send(std::as_bytes(std::span{buf}));
				m_currentState = State::menu;
				break;

			case(State::menu):

				/*
				cli.listen();
				while(!m_uartComplete){
					HAL_Delay(10);
				}
				m_uartComplete = false;
				*/
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
//				if(firstBreath){
//					m_motor.reverse();
//					m_motor.setSpeed(520);
//					firstBreath = false;
//				}
//				m_adc.startConversionInterrupt();

				//check for end detection
				if(m_endFlag) {
					m_motor.stop();
//					m_uart.send(std::as_bytes(std::span{"\nEndposition: "}));
//					std::snprintf(buf, std::size(buf)-1, "%d", m_adc.readValue());
//					m_uart.send(std::as_bytes(std::span{buf}));
//					for(int i {0}; i<10; ++i){
//						buf[i] = '\0';
//					}
//
//					while(1){
//						if(m_pinout.m_blueButton.read()){
//							m_motor.forward();
//							m_endFlag = false;
//							break;
//						}
//					}
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
							m_motor.stop();
							m_currentState = State::stop;
							break;
						}
					}
					//calculate new position measurement in mm
					position = calcPosition(m_adc.readValue());
					m_adcComplete = false;
					//get correct sample
					if(time>pattern::sinePattern[sample].time) {
						++sample;
					}
					//calculate control update
					speed = pid.update(pattern::sinePattern[sample].position, position);
					//LOG DATA HERE
					//direction logic
					if(speed < 0) {
						m_motor.reverse();
						speed = -speed;
					}
					else{
						m_motor.forward();
					}
					m_motor.setSpeed(speed);

					//logic to start pattern from the beginning if end was reached
					if(time==pattern::sinePattern[maxSample].time){
						time = 1;
						sample = 1;
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
				m_motor.stop();
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
float Application::calcPosition(uint16_t adc) {
	return adc*constants::lsbLength-13.23737f;
}

void Application::CLIversion()  {
	m_uart.send(std::as_bytes(std::span{"Version: 0.7.1\r\n"}));
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
	m_currentState = State::init;
	//menu first entry = true;
}

#include "Application.hpp"
#include "Cli.hpp"
#include "Command.hpp"
#include "Pid.hpp"
#include "sine6.hpp"
#include <cstdio>
#include <cmath>

Application::Application(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc,
													TIM_HandleTypeDef* TIMhandle)
	:m_pinout {}, m_uart {uart, dma}, m_adc {adc}, m_motor {m_pinout.m_dir, TIMhandle}  {
		std::copy(pattern::sineSix.begin(), pattern::sineSix.end(), m_breathingPattern.data.begin());
		m_breathingPattern.length = pattern::sineSix.size();
		m_step = 1.0;	//implement elsewhere!
		m_endTime = 60000/6;	//specific for sineSix
}

Application& Application::getInstance(UART_HandleTypeDef*  uart, DMA_HandleTypeDef* dma,
																			ADC_HandleTypeDef* adc, TIM_HandleTypeDef* TIMhandle) {
	static Application m_instance {uart, dma, adc, TIMhandle};
	return m_instance;
}


extern Application& application;
void Application::loop() {
	CmdVersion cmdversion{application};
	CmdBlink cmdblink{application};
	CmdBreathe cmdbreathe{application};
	CmdSelect cmdselect{application};
	CmdPause cmdpause{application};
	CmdEndpos cmdendpos{application};
	std::array commands{
		static_cast<Command*>(&cmdversion),
		static_cast<Command*>(&cmdblink),
		static_cast<Command*>(&cmdbreathe),
		static_cast<Command*>(&cmdselect),
		static_cast<Command*>(&cmdpause),
		static_cast<Command*>(&cmdendpos),
	};
	Cli cli {commands, m_uart, m_uartSize, application};
	bool menuFirstEntry {true};
	PID pid {120.0f, 0.0f, 0.0f, 0.1f, -999, 999, -200.0f, 200.0f, 0.001f};
	std::size_t sample {0};
	int speed {0};
	//time in ms
	unsigned short time {0};
	float position {0.0f};
	char buf[20] {};
	int glidingSampleRoundedDown {0};
	float glidingSample {0.0f};
	float interpolPosition {m_breathingPattern.data[0]};


	while(1) {
		switch(m_currentState){
			case(State::init):
			{
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
					m_currentState = State::stop;
					break;
				}
				//do regulation
				if(m_regTimer) {
					//timing Pin
					m_pinout.m_timerPin.set();
					m_adc.startConversionInterrupt();
					//wait for new ADC value
					while(!m_adcComplete) {
						if(m_endFlag) {
							m_currentState = State::stop;
							break;
						}
					}
					//calculate new position measurement in mm
					position = m_calcPosition(m_adc.readValue());
					m_adcComplete = false;
					//get correct sample
					if(time > sample*10){
					            ++sample;
					            glidingSample = sample * m_step;
					            glidingSampleRoundedDown = static_cast<int>(glidingSample);
					            if(glidingSampleRoundedDown >= m_breathingPattern.length-1){
					            	interpolPosition = m_breathingPattern.data[m_breathingPattern.length-1];
					                break;
					            }
					            else{
					            	interpolPosition = std::lerp(m_breathingPattern.data[glidingSampleRoundedDown], m_breathingPattern.data[glidingSampleRoundedDown+1], glidingSample-glidingSampleRoundedDown);
					            }
					        }
					//calculate control update
					speed = pid.update(interpolPosition, position);
					//direction logic
					if(speed < 0) {
						m_motor.reverse();
						m_motor.setSpeed(-speed);
					}
					else{
						m_motor.forward();
						m_motor.setSpeed(speed);
					}

					//LOG DATA HERE
					if(constants::LogPID) {
						if(m_breathCounter < 3) {
							std::snprintf(buf, std::size(buf), "%6.2f,%6.2f,%+4i\n", interpolPosition, position, speed);
							m_uart.send1(buf, 19);
						}
					}

					//logic to start pattern from the beginning if end was reached
					if(time ==  m_endTime){
						time = 1;
						sample = 0;
						++m_breathCounter;
					}
					else{
						++time;
					}
					m_regTimer = false;

					//timing Pin
					m_pinout.m_timerPin.clear();
				}

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

void Application::m_directionTest() {
	m_motor.forward();
	m_motor.setSpeed(600);
	HAL_Delay(400);
	m_motor.reverse();
	HAL_Delay(400);
	m_motor.stop();
}

void Application::m_moveEndToEnd(int speed, unsigned int lowerEnd, unsigned int upperEnd) {
	m_motor.setSpeed(speed);
	m_adc.startConversionInterrupt();
	if(m_adcComplete) {
		uint32_t adcRaw = m_adc.readValue();
		m_adcComplete = false;
		if(adcRaw < lowerEnd){
			m_motor.forward();
		}
		else if(adcRaw > upperEnd){
			m_motor.reverse();
		}
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

void Application::CLIselect() {
	m_uart.send(std::as_bytes(std::span{"The following patterns are available:\n"}));
	m_uart.send(std::as_bytes(std::span{"0: Abort\n"}));
	m_uart.send(std::as_bytes(std::span{"1: Sine6\n"}));

	uint8_t buffer[5] {};
	bool found {false};
	m_uartComplete = false;
	while(!found) {
		m_uart.receiveToIdleDMA(buffer, std::size(buffer));
		while(!m_uartComplete) {
			HAL_Delay(20);
			//no optimization!
		}
		HAL_Delay(5);
		m_uartComplete = false;
		switch(buffer[0]) {
			case '0':
				m_uart.send(std::as_bytes(std::span{"Aborted\n"}));
				return;
			case '1':
				std::copy(pattern::sineSix.begin(), pattern::sineSix.end(), m_breathingPattern.data.begin());
				m_breathingPattern.length = pattern::sineSix.size();
				found = true;
				break;
			default:
				m_uart.send(std::as_bytes(std::span{"Please enter a vaild number\n"}));
				for(std::size_t i{0}; i < std::size(buffer); ++i) {
					buffer[i] = '\0';
				}
				break;
		}
	}
	m_breathCounter = 0;
	m_step = m_requested_freq / m_breathingPattern.frequency;
	m_endTime = 60000 / m_requested_freq;
	buffer[1] = '\n';
	m_uart.send(std::as_bytes(std::span{"Successfully switched to  number "}));
	m_uart.send(std::as_bytes(std::span{buffer, 2}));
}

void Application::CLIpause() {
	m_motor.stop();
	m_currentState = State::menu;
	//menu first entry = true;
}

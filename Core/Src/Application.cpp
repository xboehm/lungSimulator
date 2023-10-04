#include "Application.hpp"
#include "GlobalConf.hpp"
#include "Cli.hpp"
#include "Command.hpp"
#include "Pid.hpp"
#include "sine6V.hpp"
#include <cstdio>
#include <cmath>
#include <charconv>
#include <cstdlib>
#include <algorithm>

Application::Application(DMA_HandleTypeDef* dma, ADC_HandleTypeDef* adc,
											TIM_HandleTypeDef* TIMhandle, UART_HandleTypeDef*  uart)
	:m_pinout {}, m_adc {adc}, m_motor {m_pinout.m_dir, TIMhandle}, m_uart {uart, dma} {
		m_copyPattern(pattern::sineSixV);
}

Application& Application::getInstance(DMA_HandleTypeDef* dma, 	ADC_HandleTypeDef* adc,
																	TIM_HandleTypeDef* TIMhandle, UART_HandleTypeDef*  uart) {
	static Application m_instance {dma, adc, TIMhandle, uart};
	return m_instance;
}


extern Application& application;
void Application::loop() {
	std::array<Command*, 10> commands {};
	Cli cli {commands, m_uart, m_uartSize, application};
	CmdVersion cmdversion{application};
	commands.at(0) = &cmdversion;
	CmdBlink cmdblink{application};
	commands.at(1)  = &cmdblink;
	CmdBreathe cmdbreathe{application};
	commands.at(2) = &cmdbreathe;
	CmdSelect cmdselect{application};
	commands.at(3) = &cmdselect;
	CmdPause cmdpause{application};
	commands.at(4) = &cmdpause;
	CmdEndpos cmdendpos{application};
	commands.at(5) = &cmdendpos;
	CmdFreq cmdfreq{application, cli.getPayload()};
	commands.at(6) = &cmdfreq;
	CmdVol cmdvol {application, cli.getPayload()};
	commands.at(7) = &cmdvol;
	CmdChange cmdchange {application, cli.getPayload()};
	commands.at(8) = &cmdchange;
	CmdFeed cmdfeed {application};
	commands.at(9) = &cmdfeed;

	bool menuFirstEntry {true};
	PID pid {120.0f, 0.0f, 0.0f, 0.1f, -999, 999, -200.0f, 200.0f, 0.001f};
	std::size_t sample {0};
	int speed {0};
	//time in ms
	unsigned short time {0};
	float position {0.0f};
	char buf[20] {};
	unsigned int glidingSampleRoundedDown {0};
	float glidingSample {0.0f};
	float interpolPosition {m_breathingPattern.data[0]};
	std::size_t ii {0};



	while(1) {
		switch(m_currentState){
			case(State::init):
			{
					menuFirstEntry = true;
//					char testC {'c'};
//					m_uart << "Hello world! " << 69  << 'a' << "b" << testC << 555 << '\n';
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
					            glidingSampleRoundedDown = static_cast<unsigned int>(glidingSample);
					            if(glidingSampleRoundedDown >= m_breathingPattern.length-1){
					            	interpolPosition = m_breathingPattern.data[m_breathingPattern.length-1];
					                break;
					            }
					            else{
					            	interpolPosition = std::lerp(m_breathingPattern.data[glidingSampleRoundedDown], m_breathingPattern.data[glidingSampleRoundedDown+1], glidingSample-glidingSampleRoundedDown);
					            }
					        }
					//calculate control update
					speed = pid.update((interpolPosition*m_positionFactor*m_volFactor)+25, position);
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
							m_uart << std::span(&buf[0], 19);
						}
					}

					//logic to start pattern from the beginning if end was reached
					if(time >=  m_endTime){
						m_motor.stop();
						time = 1;
						sample = 0;
						++m_breathCounter;
						//if breathing parameters got changed by external command
						if(m_paramChange){
							//set requested freq from external command
							m_endTime = 60000/m_requestedFreq;
							m_step = m_requestedFreq / m_breathingPattern.frequency;
							//set requested volume form external command
							m_volFactor = static_cast<float>(m_requestedVolume)/m_breathingPattern.volume;
							m_breathCounter = 0;
							m_paramChange = false;
						}
						if(m_diffPattern){
							m_copyPattern(std::span(m_inputPattern.data.begin(), m_inputPattern.length));
							m_breathCounter = 0;
							m_diffPattern = false;
						}
					}
					else{
						++time;
					}
					m_regTimer = false;

					//timing Pin
					m_pinout.m_timerPin.clear();
				}

				//converting char input from serial to floats and storing it in m_inputPattern
				if(m_process){
//					while(!m_regTimer){
						m_inputPattern.data[ii++] = static_cast<float>(std::atof(reinterpret_cast<char*>(index)));
						index = std::find(index, patternInput.end(), ';');
						if(index == patternInput.end()){
							m_process = false;
							m_inputPattern.length = ii;
							m_inpAvail = true;
							cli.listen();
//							break;
						}
						else{
							++index;
						}
//					}
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
	while(1) {
		raw = m_adc.readSinglePoll();
		m_uart << raw << '\n';
		HAL_Delay(delay);
	}
}

void Application::m_readADCbuffer() {
	std::array<uint32_t, constants::ADCreads> buffer {};
	m_adc.readSingleLoop(2U, buffer);
	for(auto element: buffer){
		m_uart << element << '\n';
	}
}

void Application::m_buttonTest(){
	while(1) {
		m_uart << m_pinout.m_blueButton.read() << ' ' << m_pinout.m_endO.read() << ' ' << m_pinout.m_endC.read() << '\n';
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
	return adc*constants::LsbLength-13.23737f;
}

void Application::m_stopMotor() {
	m_motor.stop();
}

void Application::m_getEndPositions() {
	m_motor.reverse();
	m_motor.setSpeed(380);
	//wait for end detection;
	while(!m_endFlag){
		//no optimization!
	}
	uint32_t endPosClosed {m_adc.readSinglePoll()};
	HAL_Delay(10);
	m_motor.forward();
	HAL_Delay(200);
	m_endFlag = false;
	//wait for end detection;
	while(!m_endFlag){
		//no optimization!
	}
	uint32_t endPosOpen {m_adc.readSinglePoll()};
	HAL_Delay(10);
	m_motor.reverse();
	HAL_Delay(300);
	m_motor.stop();
	m_endFlag = false;
	m_uart << "Endpositions: " << endPosClosed <<  ", " << endPosOpen << '\n';
}

void Application::m_toggleTimerPin() {
	m_pinout.m_timerPin.clear();
}

void Application::m_copyPattern(std::span<const float> data){
	auto minmax {std::minmax_element(data.begin(), data.end())};
	auto newVol {static_cast<int>(*minmax.second - *minmax.first)};
	if(newVol>=0 && newVol<=static_cast<int>(0.92*m_cylVolume)){
		 m_breathingPattern.volume = newVol;
	}
	else{
		 m_uart << "Requested volume out of range.\n";
		 return;
	 }
	auto newFreq {6000.0/(data.size()-1)};
	if(newFreq >= constants::MinimalFreq && newFreq <= constants::MaximalFreq){
		m_breathingPattern.frequency = newFreq;
	}
	else{
		m_uart << "Requested frequency out of range.\n";
		return;
	}
	 std::copy(data.begin(), data.end(), m_breathingPattern.data.begin());
	 m_breathingPattern.length = data.size();	//watch it!
	 m_requestedFreq = m_breathingPattern.frequency;
	 m_endTime = (data.size()-1)*10;
	 m_requestedVolume = m_breathingPattern.volume;
	 m_step = m_requestedFreq / m_breathingPattern.frequency;	  //could also just set it to 1.0
	 m_volFactor = static_cast<float>(m_requestedVolume)/m_breathingPattern.volume; //could also just set it to 1.0
	 ++newVol;
}

void Application::CLIversion()  {
	m_uart << "Version: 1.0\n";
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
	m_uart << "The following patterns are available:\n";
	m_uart << "0: Abort\n";
	if(m_inpAvail){
			m_uart << "1: Input\n";
	}
	m_uart << "2: Sine6V\n";


	uint8_t buffer[5] {};
	bool found {false};
	m_uartComplete = false;
	while(!found) {
		m_uart.receiveToIdleDMA(buffer, std::size(buffer));
		while(!m_uartComplete) {
			HAL_Delay(5);
			//no optimization!
		}
//		HAL_Delay(5);
		m_uartComplete = false;
		switch(buffer[0]) {
			case '0':
				m_uart << "Aborted\n";
				return;
			case '1':
				if(m_inpAvail){
//					m_copyPattern(std::span(m_inputPattern.data.begin(), m_inputPattern.length));
					m_diffPattern = true;
					found = true;
				}
				else{
					m_uart << "Please enter a valid number\n";
					for(std::size_t i{0}; i < std::size(buffer); ++i) {
						buffer[i] = '\0';
					}
				}
				break;
			case '2':
				m_copyPattern(pattern::sineSixV);
				found = true;
				return;
			default:
				m_uart << "Please enter a valid number\n";
				for(std::size_t i{0}; i < std::size(buffer); ++i) {
					buffer[i] = '\0';
				}
				break;
		}
	}
//	m_breathCounter = 0;
//	m_step = m_requestedFreq / m_breathingPattern.frequency;
//	m_endTime = 60000 / m_requestedFreq;
	m_uart << "Successfully switched to  number " << static_cast<char>(buffer[0]) << " with next breath.\n";
}

void Application::CLIpause() {
	m_motor.stop();
	m_currentState = State::menu;
	//menu first entry = true;
}

void Application::CLIfreq(CommandPayload& payload){
	float newFreq {static_cast<float>(std::atof(payload.data()))};		//alternative to std::from_chars() until it is supported for floating point types
	if(newFreq >= constants::MinimalFreq && newFreq <= constants::MaximalFreq){
		m_requestedFreq = newFreq;
		m_paramChange = true;
	}
	else{
		m_uart << "Requested frequency out of range.\n";
	}
	//	std::from_chars(payload.data(), payload.data()+payload.size(), m_requested_freq);	//won't work with floating points as of "GNU Tools for STM32 (11.3.rel1)"

}

void Application::CLIvol(CommandPayload& payload){
	int newVol {};
    std::from_chars(payload.begin(), payload.end(), newVol);
    if(newVol>=0 && newVol<=static_cast<int>(0.92*m_cylVolume)){
    	m_requestedVolume = newVol;
    	m_uart << "Breath volume changed to " << newVol << '\n';
    	m_paramChange = true;
    }
    else{
    		m_uart << "Requested volume out of range.\n";
    	}
}

void Application::CLIchange(CommandPayload& payload){
	int radius {};
	int length {};
	auto hyphenPos {std::find(payload.begin(), payload.end(), '-')};
	std::from_chars(payload.begin(), payload.end(), radius);
	std::from_chars(hyphenPos+1, payload.end(), length);
	m_positionFactor = 1000/(constants::Pi*radius*radius);
	m_cylVolume = (constants::Pi*radius*radius*length)/1000;
}

void Application::CLIfeed() {
	m_uart << "Hungry now.\n";
	m_uart.receiveDMA(index, 1);
//	m_feed = true;

//	m_uart.abortRx();
}

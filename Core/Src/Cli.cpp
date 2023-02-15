#include "Cli.hpp"

void Cli::version(Application* application) {
	application->m_uart.send(std::as_bytes(std::span{"Version: 0.7.1\r\n"}));
}

void Cli::blink(Application* application) {
	while(1){
		application->m_pinout.m_onboardLed.toggle();
		HAL_Delay(500);
	}
}

void Cli::breathe(Application* application) {
	application->m_currentState = Application::State::breathe;
}

void Cli::select(Application* application) {

}

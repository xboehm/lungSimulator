#include "Pinout.hpp"
#include "stm32l4xx_hal.h"

// Wrap all GPIO pins / ports into project from main.h
Pinout::Pinout()
	: m_onboardLed {LD4_GPIO_Port, LD4_Pin},
	  m_blueButton {B1_GPIO_Port, B1_Pin},
	  m_dir {DIR_GPIO_Port, DIR_Pin},
	  m_endC {endC_GPIO_Port, endC_Pin},
	  m_endO {endO_GPIO_Port, endO_Pin},
	  m_timerPin {TimerPin_GPIO_Port, TimerPin_Pin}
{
}

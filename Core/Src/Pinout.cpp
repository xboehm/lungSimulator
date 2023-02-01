#include "Pinout.hpp"
#include "stm32l4xx_hal.h"

// Wrap all GPIO pins / ports into project from main.h
Pinout::Pinout()
	: m_onboardLed {LD4_GPIO_Port, LD4_Pin},
	  m_blueButton {B1_GPIO_Port, B1_Pin},
	  m_dir {DIR_GPIO_Port, DIR_Pin},
	  m_endR {endR_GPIO_Port, endR_Pin}
{
}

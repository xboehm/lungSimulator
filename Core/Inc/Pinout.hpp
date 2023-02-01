#pragma once

#include "GpioPin.hpp"
#include "main.h"

class Pinout{
public:
	//Define all wanted GPIO pins for specific project below
	GpioPin m_onboardLed;
	GpioPin m_blueButton;
	GpioPin m_dir;
	GpioPin m_endR;

private:
	friend class Application;
	Pinout();
};

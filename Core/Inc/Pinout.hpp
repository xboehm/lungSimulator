#pragma once

#include "GpioPin.hpp"
#include "main.h"

class Pinout{
public:
	//Define all wanted GPIO pins for specific project below
	GpioPin m_onboardLed;
	GpioPin m_dir;

private:
	friend class Application;
	Pinout();
};

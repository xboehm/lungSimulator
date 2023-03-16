#pragma once

#include "GlobalConf.hpp"
#include <array>

struct BreathPoint {
		unsigned short time;
		float position;
	};

struct BreathingPattern {
	std::array<BreathPoint, constants::MaxBreathPoints> data;
	std::size_t length {0};
	float period {0.0f};
	float volume {0.0f};
};

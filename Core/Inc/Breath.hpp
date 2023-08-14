#pragma once

#include "GlobalConf.hpp"
#include <array>

struct BreathingPattern {
	std::array<float, constants::MaxBreathPoints> data;
	std::size_t length {0};
	float period {0.0f};
	float volume {0.0f};
	float frequency {0.0f};
};

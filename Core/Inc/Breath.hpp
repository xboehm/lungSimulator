#pragma once

#include "GlobalConf.hpp"
#include <array>

struct BreathingPattern {
	std::array<float, constants::MaxBreathPoints> data;
	std::size_t length {0};
	int volume {0};
	float frequency {0.0f};
};

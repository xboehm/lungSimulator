#pragma once

#include <cstddef>

namespace constants{
	constexpr int ADCreads {100};
	constexpr std::size_t RxBufSize {15};
	constexpr float LsbLength {0.048818f};
	constexpr std::size_t MaxBreathPoints {1001};
	constexpr bool LogPID {false};
	constexpr float Pi {3.141592f};
	constexpr float MinimalFreq {6.0f};
	constexpr float MaximalFreq {25.0f};
}

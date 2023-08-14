#pragma once

#include <cstddef>

namespace constants{
	constexpr int ADCreads {2000};
	constexpr std::size_t RxBufSize {15};
	constexpr float lsbLength {0.048818f};
	constexpr std::size_t MaxBreathPoints {1001};
	constexpr bool LogPID {true};
}

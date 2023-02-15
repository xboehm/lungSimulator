#pragma once

#include "Application.hpp"
#include <array>
#include <string_view>

class Application;
class Cli{
private:
	struct Command{
		std::string_view name;
		std::string_view help;
	    void(*execute)(Application* application);
	};

	static void version(Application* application);
	static void blink(Application* application);
	static void breathe(Application* application);
	static void select(Application* application);
public:
	static constexpr std::array commands {
					Command{"version", "Display firmware version", version},
					Command{"blink", "Blinks LD4 at 10Hz", blink},
					Command{"breathe", "start breathing cycle", breathe},
					Command{"select", "select a breathing pattern to run", select},
	};
};


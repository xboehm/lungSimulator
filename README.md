# lungSimulator
This repo contains the firmware for my master thesis. 
The application controls a self-developed low-cost lung simulator.
The simulator will be used in a medical lab at Pforzheim University to create various realistic human breathing patterns.
It will help to teach students the use of mechanical ventilators without the risk of injuring a human lung.

The project runs bare-metal on a STM32L433RC-P µC and uses the ST HAL drivers under the hood.
The high-level application is written in C++20.
The peripherals used include
- ADC
- DMA
- GPIO
- NVIC (nested vector interrupt controller)
- PWM
- UART

The lung simulator is composed of a pneumatic cylinder, driven by a linear actuator.
When the actuator moves the piston of the cylinder, an air flow results at the outlet.
A PID-control-algorithm uses the position of the piston and set points defined by a breathing pattern to drive the actuator, 
in order that the desired breathing pattern gets generated at the outlet.
The user can choose from different predefined breathing patterns or feed new pattern data to the simulator via the UART interface.
All patterns can be scaled variably in their frequency and volume.

## How to import the project
Download the STM32 Cube IDE, copy the repo into the designated workspace folder and open the `.project` file.

## How to use the project
To communicate with the simulator the UART interface is used.
To establish a connection between the dev board and a computer the micro-USB port of the onboard debugger 
needs to be connected with an USB port of the computer.
Alternatively, the UART pins of the USART2 interface on the board can be used directly.
RX pin is routed to `PA_3`, TX pin is routed to `PA_2`.
See [board pinout](https://os.mbed.com/platforms/NUCLEO-L433RC-P/) for reference.

Following configuration is used:
* Baud rate: 460800
* Data bits: 8
* Stop bits: 1
* Parity: none

To check if the communication is set up correctly the command 'help' can be send.
The expected answer is a list conatining all available command names with a short description.

### Commands
A standard breathing pattern is loaded at initialization.
To start breathing the command `breathe` needs to be send.
Breathing can be paused with `pause` and continued with `breathe` again.
The command `freq-10` will change the frequency of the pattern to 10bpm (beats per minute) starting with the next breath.
The command `vol-620` will change the volume of the pattern to 620ml tidal volume starting with the next breath.
The command `select-2` selects the second hard coded breathing pattern and starts with its default frequency and volume.
The change will be made after the current breath has finished.

A complete list of all available commands and a short description can be found below:
Command name | Description
--------------------|-------------------
help | show all available commands
version |print current firmware version
breathe |start breathing cycle
pause |interrupt breathing cycle
feed |feed the simulator with new pattern data
select-3 |select third breathing pattern
freq-22 |play pattern with 22bpm [^1]
vol-580 |play pattern with 580ml tidal volume [^2]
cyl-50-200 |cylinder change: radius 50mm, length 200mm

[^1]: (accepts values between 6 and 25)
[^2]: (accepts values between 0 and 92% of maximum cylinder volume)

### Breathing patterns
A breathing pattern is a discrete volume flow, sampled at 100Hz.
It consists of an array of volume points in the milliliters, each 10ms apart from another.
The firmware uses floats to store the volume points.
The simulator is tested to work with patterns from 6bpm to 25bpm.
Breathing patterns can have a maximum of 1001 volume points, which maps to a pattern witch 6bpm.
An example of a hard coded breathing pattern can be found at `Core/Inc/sine6v.hpp`.
To add new hard coded patterns, follow these instructions:
1. Copy `Core/Inc/sine6v.hpp`
2. Change pattern name from `sineSixV` (line 7) to new desired name
3. Replace old datapoints inside the array with new ones
4. Paste new file with different name in the `Core/Inc` directory
5. Include new file with `#include "new_file_name.hpp"` in `Core/Src/Application.cpp`
6. Create new case inside switch statement of `CLIselect` function inside `Core/Src/Application.cpp` to make new pattern selectable, use `m_copyPattern()` function

### Feed new pattern data
The simulator is able to receive new pattern data via the UART interface while breathing.
To start the reception, the command `feed` needs to be called during breathing.
The data must meet following criteria:
- Each volume point must be in the format `XXX.YY`, left-hand zeros may be omitted
- Volume points are separated by a semicolon (`;`)
- No white spaces bewtween different volume points or a maximum of 6 characters between two semicolons
- After the last volume point is a exclamation point (`!`) to mark the end of the data set
An example file containing correct data to be fed can be found [here](Misc/sine10V.txt).

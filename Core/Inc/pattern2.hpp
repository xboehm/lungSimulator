#pragma once

#include "Breath.hpp"
#include <array>

namespace pattern{
	static constexpr std::array<BreathPoint, 246> two {
	  BreathPoint{0, 63}, BreathPoint{10, 63.03}, BreathPoint{20, 63.11}, BreathPoint{30, 63.26}, BreathPoint{40, 63.48}, BreathPoint{50, 63.8}, BreathPoint{60, 64.18}, BreathPoint{70, 64.61}, BreathPoint{80, 65.07}, BreathPoint{90, 65.54},
	  BreathPoint{100, 66.02}, BreathPoint{110, 66.51}, BreathPoint{120, 67.01}, BreathPoint{130, 67.53}, BreathPoint{140, 68.07}, BreathPoint{150, 68.62}, BreathPoint{160, 69.18}, BreathPoint{170, 69.77}, BreathPoint{180, 70.37}, BreathPoint{190, 70.98},
	  BreathPoint{200, 71.61}, BreathPoint{210, 72.25}, BreathPoint{220, 72.88}, BreathPoint{230, 73.53}, BreathPoint{240, 74.18}, BreathPoint{250, 74.83}, BreathPoint{260, 75.47}, BreathPoint{270, 76.11}, BreathPoint{280, 76.76}, BreathPoint{290, 77.41},
	  BreathPoint{300, 78.07}, BreathPoint{310, 78.73}, BreathPoint{320, 79.39}, BreathPoint{330, 80.03}, BreathPoint{340, 80.67}, BreathPoint{350, 81.31}, BreathPoint{360, 81.95}, BreathPoint{370, 82.59}, BreathPoint{380, 83.23}, BreathPoint{390, 83.88},
	  BreathPoint{400, 84.53}, BreathPoint{410, 85.18}, BreathPoint{420, 85.83}, BreathPoint{430, 86.48}, BreathPoint{440, 87.13}, BreathPoint{450, 87.76}, BreathPoint{460, 88.4}, BreathPoint{470, 89.05}, BreathPoint{480, 89.68}, BreathPoint{490, 90.31},
	  BreathPoint{500, 90.92}, BreathPoint{510, 91.53}, BreathPoint{520, 92.14}, BreathPoint{530, 92.74}, BreathPoint{540, 93.33}, BreathPoint{550, 93.92}, BreathPoint{560, 94.48}, BreathPoint{570, 95.03}, BreathPoint{580, 95.57}, BreathPoint{590, 96.1},
	  BreathPoint{600, 96.63}, BreathPoint{610, 97.17}, BreathPoint{620, 97.71}, BreathPoint{630, 98.23}, BreathPoint{640, 98.74}, BreathPoint{650, 99.26}, BreathPoint{660, 99.79}, BreathPoint{670, 100.34}, BreathPoint{680, 100.89}, BreathPoint{690, 101.45},
	  BreathPoint{700, 102}, BreathPoint{710, 102.54}, BreathPoint{720, 103.08}, BreathPoint{730, 103.62}, BreathPoint{740, 104.15}, BreathPoint{750, 104.67}, BreathPoint{760, 105.19}, BreathPoint{770, 105.7}, BreathPoint{780, 106.21}, BreathPoint{790, 106.71},
	  BreathPoint{800, 107.22}, BreathPoint{810, 107.73}, BreathPoint{820, 108.23}, BreathPoint{830, 108.72}, BreathPoint{840, 109.19}, BreathPoint{850, 109.65}, BreathPoint{860, 110.09}, BreathPoint{870, 110.52}, BreathPoint{880, 110.95}, BreathPoint{890, 111.36},
	  BreathPoint{900, 111.78}, BreathPoint{910, 112.19}, BreathPoint{920, 112.59}, BreathPoint{930, 112.98}, BreathPoint{940, 113.37}, BreathPoint{950, 113.74}, BreathPoint{960, 114.11}, BreathPoint{970, 114.47}, BreathPoint{980, 114.83}, BreathPoint{990, 115.18},
	  BreathPoint{1000, 115.52}, BreathPoint{1010, 115.83}, BreathPoint{1020, 116.07}, BreathPoint{1030, 116.26}, BreathPoint{1040, 116.42}, BreathPoint{1050, 116.54}, BreathPoint{1060, 116.58}, BreathPoint{1070, 116.58}, BreathPoint{1080, 116.58}, BreathPoint{1090, 116.58},
	  BreathPoint{1100, 116.58}, BreathPoint{1110, 116.58}, BreathPoint{1120, 116.58}, BreathPoint{1130, 116.58}, BreathPoint{1140, 116.58}, BreathPoint{1150, 116.58}, BreathPoint{1160, 116.58}, BreathPoint{1170, 116.58}, BreathPoint{1180, 116.58}, BreathPoint{1190, 116.58},
	  BreathPoint{1200, 116.58}, BreathPoint{1210, 116.55}, BreathPoint{1220, 116.47}, BreathPoint{1230, 116.33}, BreathPoint{1240, 116.14}, BreathPoint{1250, 115.9}, BreathPoint{1260, 115.61}, BreathPoint{1270, 115.29}, BreathPoint{1280, 114.96}, BreathPoint{1290, 114.59},
	  BreathPoint{1300, 114.18}, BreathPoint{1310, 113.74}, BreathPoint{1320, 113.28}, BreathPoint{1330, 112.77}, BreathPoint{1340, 112.24}, BreathPoint{1350, 111.7}, BreathPoint{1360, 111.16}, BreathPoint{1370, 110.59}, BreathPoint{1380, 109.99}, BreathPoint{1390, 109.36},
	  BreathPoint{1400, 108.7}, BreathPoint{1410, 108.02}, BreathPoint{1420, 107.32}, BreathPoint{1430, 106.61}, BreathPoint{1440, 105.88}, BreathPoint{1450, 105.14}, BreathPoint{1460, 104.41}, BreathPoint{1470, 103.69}, BreathPoint{1480, 102.98}, BreathPoint{1490, 102.29},
	  BreathPoint{1500, 101.61}, BreathPoint{1510, 100.93}, BreathPoint{1520, 100.25}, BreathPoint{1530, 99.56}, BreathPoint{1540, 98.88}, BreathPoint{1550, 98.19}, BreathPoint{1560, 97.51}, BreathPoint{1570, 96.84}, BreathPoint{1580, 96.19}, BreathPoint{1590, 95.53},
	  BreathPoint{1600, 94.87}, BreathPoint{1610, 94.2}, BreathPoint{1620, 93.56}, BreathPoint{1630, 92.94}, BreathPoint{1640, 92.32}, BreathPoint{1650, 91.72}, BreathPoint{1660, 91.13}, BreathPoint{1670, 90.55}, BreathPoint{1680, 89.97}, BreathPoint{1690, 89.38},
	  BreathPoint{1700, 88.77}, BreathPoint{1710, 88.17}, BreathPoint{1720, 87.57}, BreathPoint{1730, 86.99}, BreathPoint{1740, 86.42}, BreathPoint{1750, 85.85}, BreathPoint{1760, 85.27}, BreathPoint{1770, 84.69}, BreathPoint{1780, 84.11}, BreathPoint{1790, 83.53},
	  BreathPoint{1800, 82.95}, BreathPoint{1810, 82.38}, BreathPoint{1820, 81.82}, BreathPoint{1830, 81.3}, BreathPoint{1840, 80.79}, BreathPoint{1850, 80.29}, BreathPoint{1860, 79.79}, BreathPoint{1870, 79.29}, BreathPoint{1880, 78.8}, BreathPoint{1890, 78.3},
	  BreathPoint{1900, 77.79}, BreathPoint{1910, 77.3}, BreathPoint{1920, 76.81}, BreathPoint{1930, 76.32}, BreathPoint{1940, 75.84}, BreathPoint{1950, 75.36}, BreathPoint{1960, 74.89}, BreathPoint{1970, 74.43}, BreathPoint{1980, 73.98}, BreathPoint{1990, 73.51},
	  BreathPoint{2000, 73.05}, BreathPoint{2010, 72.59}, BreathPoint{2020, 72.15}, BreathPoint{2030, 71.72}, BreathPoint{2040, 71.29}, BreathPoint{2050, 70.88}, BreathPoint{2060, 70.46}, BreathPoint{2070, 70.03}, BreathPoint{2080, 69.60}, BreathPoint{2090, 69.17},
	  BreathPoint{2100, 68.77}, BreathPoint{2110, 68.36}, BreathPoint{2120, 67.97}, BreathPoint{2130, 67.59}, BreathPoint{2140, 67.2}, BreathPoint{2150, 66.84}, BreathPoint{2160, 66.48}, BreathPoint{2170, 66.14}, BreathPoint{2180, 65.81}, BreathPoint{2190, 65.48},
	  BreathPoint{2200, 65.16}, BreathPoint{2210, 64.84}, BreathPoint{2220, 64.56}, BreathPoint{2230, 64.29}, BreathPoint{2240, 64.04}, BreathPoint{2250, 63.8}, BreathPoint{2260, 63.57}, BreathPoint{2270, 63.4}, BreathPoint{2280, 63.28}, BreathPoint{2290, 63.16},
	  BreathPoint{2300, 63.07}, BreathPoint{2310, 63.03}, BreathPoint{2320, 63}, BreathPoint{2330, 63}, BreathPoint{2340, 63}, BreathPoint{2350, 63}, BreathPoint{2360, 63}, BreathPoint{2370, 63}, BreathPoint{2380, 63}, BreathPoint{2390, 63},
	  BreathPoint{2400, 63}, BreathPoint{2410, 63}, BreathPoint{2420, 63}, BreathPoint{2430, 63}, BreathPoint{2440, 63}, BreathPoint{2450, 63},
	};
}

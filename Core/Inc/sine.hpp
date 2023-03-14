#pragma once

#include <array>

namespace pattern{
	struct BreathPoint {
		unsigned short time;
		float position;
	};

	static constexpr std::array<BreathPoint, 351> sine {
	  BreathPoint{     0,  30.00}, BreathPoint{   10,  30.01}, BreathPoint{  20,  30.03}, BreathPoint{  30,  30.07}, BreathPoint{  40,  30.13}, BreathPoint{  50,  30.20}, BreathPoint{  60,  30.29}, BreathPoint{  70,  30.39}, BreathPoint{  80,  30.51}, BreathPoint{  90,  30.65},
	  BreathPoint{ 100,  30.80}, BreathPoint{ 110,  30.97}, BreathPoint{ 120,  31.16}, BreathPoint{ 130,  31.36}, BreathPoint{ 140,  31.57}, BreathPoint{ 150,  31.80}, BreathPoint{ 160,  32.05}, BreathPoint{ 170,  32.31}, BreathPoint{ 180,  32.59}, BreathPoint{ 190,  32.88},
	  BreathPoint{ 200,  33.19}, BreathPoint{ 210,  33.51}, BreathPoint{ 220,  33.85}, BreathPoint{ 230,  34.20}, BreathPoint{ 240,  34.57}, BreathPoint{ 250,  34.95}, BreathPoint{ 260,  35.35}, BreathPoint{ 270,  35.76}, BreathPoint{ 280,  36.18}, BreathPoint{ 290,  36.62},
	  BreathPoint{ 300,  37.08}, BreathPoint{ 310,  37.54}, BreathPoint{ 320,  38.03}, BreathPoint{ 330,  38.52}, BreathPoint{ 340,  39.03}, BreathPoint{ 350,  39.55}, BreathPoint{ 360,  40.08}, BreathPoint{ 370,  40.63}, BreathPoint{ 380,  41.19}, BreathPoint{ 390,  41.76},
	  BreathPoint{ 400,  42.35}, BreathPoint{ 410,  42.94}, BreathPoint{ 420,  43.55}, BreathPoint{ 430,  44.17}, BreathPoint{ 440,  44.80}, BreathPoint{ 450,  45.45}, BreathPoint{ 460,  46.10}, BreathPoint{ 470,  46.77}, BreathPoint{ 480,  47.44}, BreathPoint{ 490,  48.13},
	  BreathPoint{ 500,  48.83}, BreathPoint{ 510,  49.53}, BreathPoint{ 520,  50.25}, BreathPoint{ 530,  50.98}, BreathPoint{ 540,  51.71}, BreathPoint{ 550,  52.46}, BreathPoint{ 560,  53.21}, BreathPoint{ 570,  53.97}, BreathPoint{ 580,  54.74}, BreathPoint{ 590,  55.52},
	  BreathPoint{ 600,  56.31}, BreathPoint{ 610,  57.10}, BreathPoint{ 620,  57.90}, BreathPoint{ 630,  58.71}, BreathPoint{ 640,  59.53}, BreathPoint{ 650,  60.35}, BreathPoint{ 660,  61.18}, BreathPoint{ 670,  62.01}, BreathPoint{ 680,  62.85}, BreathPoint{ 690,  63.70},
	  BreathPoint{ 700,  64.55}, BreathPoint{ 710,  65.41}, BreathPoint{ 720,  66.27}, BreathPoint{ 730,  67.13}, BreathPoint{ 740,  68.00}, BreathPoint{ 750,  68.87}, BreathPoint{ 760,  69.75}, BreathPoint{ 770,  70.63}, BreathPoint{ 780,  71.51}, BreathPoint{ 790,  72.40},
	  BreathPoint{ 800,  73.29}, BreathPoint{ 810,  74.18}, BreathPoint{ 820,  75.07}, BreathPoint{ 830,  75.97}, BreathPoint{ 840,  76.86}, BreathPoint{ 850,  77.76}, BreathPoint{ 860,  78.65}, BreathPoint{ 870,  79.55}, BreathPoint{ 880,  80.45}, BreathPoint{ 890,  81.35},
	  BreathPoint{ 900,  82.24}, BreathPoint{ 910,  83.14}, BreathPoint{ 920,  84.04}, BreathPoint{ 930,  84.93}, BreathPoint{ 940,  85.82}, BreathPoint{ 950,  86.71}, BreathPoint{ 960,  87.60}, BreathPoint{ 970,  88.49}, BreathPoint{ 980,  89.37}, BreathPoint{ 990,  90.25},
	  BreathPoint{1000,  91.13}, BreathPoint{1010,  92.00}, BreathPoint{1020,  92.87}, BreathPoint{1030,  93.73}, BreathPoint{1040,  94.60}, BreathPoint{1050,  95.45}, BreathPoint{1060,  96.30}, BreathPoint{1070,  97.15}, BreathPoint{1080,  97.99}, BreathPoint{1090,  98.82},
	  BreathPoint{1100,  99.65}, BreathPoint{1110, 100.47}, BreathPoint{1120, 101.29}, BreathPoint{1130, 102.10}, BreathPoint{1140, 102.90}, BreathPoint{1150, 103.69}, BreathPoint{1160, 104.48}, BreathPoint{1170, 105.26}, BreathPoint{1180, 106.03}, BreathPoint{1190, 106.79},
	  BreathPoint{1200, 107.55}, BreathPoint{1210, 108.29}, BreathPoint{1220, 109.03}, BreathPoint{1230, 109.75}, BreathPoint{1240, 110.47}, BreathPoint{1250, 111.17}, BreathPoint{1260, 111.87}, BreathPoint{1270, 112.56}, BreathPoint{1280, 113.23}, BreathPoint{1290, 113.90},
	  BreathPoint{1300, 114.55}, BreathPoint{1310, 115.20}, BreathPoint{1320, 115.83}, BreathPoint{1330, 116.45}, BreathPoint{1340, 117.06}, BreathPoint{1350, 117.65}, BreathPoint{1360, 118.24}, BreathPoint{1370, 118.81}, BreathPoint{1380, 119.37}, BreathPoint{1390, 119.92},
	  BreathPoint{1400, 120.45}, BreathPoint{1410, 120.97}, BreathPoint{1420, 121.48}, BreathPoint{1430, 121.97}, BreathPoint{1440, 122.46}, BreathPoint{1450, 122.92}, BreathPoint{1460, 123.38}, BreathPoint{1470, 123.82}, BreathPoint{1480, 124.24}, BreathPoint{1490, 124.65},
	  BreathPoint{1500, 125.05}, BreathPoint{1510, 125.43}, BreathPoint{1520, 125.80}, BreathPoint{1530, 126.15}, BreathPoint{1540, 126.49}, BreathPoint{1550, 126.81}, BreathPoint{1560, 127.12}, BreathPoint{1570, 127.41}, BreathPoint{1580, 127.69}, BreathPoint{1590, 127.95},
	  BreathPoint{1600, 128.20}, BreathPoint{1610, 128.43}, BreathPoint{1620, 128.64}, BreathPoint{1630, 128.84}, BreathPoint{1640, 129.03}, BreathPoint{1650, 129.20}, BreathPoint{1660, 129.35}, BreathPoint{1670, 129.49}, BreathPoint{1680, 129.61}, BreathPoint{1690, 129.71},
	  BreathPoint{1700, 129.80}, BreathPoint{1710, 129.87}, BreathPoint{1720, 129.93}, BreathPoint{1730, 129.97}, BreathPoint{1740, 129.99}, BreathPoint{1750, 130.00}, BreathPoint{1760, 129.99}, BreathPoint{1770, 129.97}, BreathPoint{1780, 129.93}, BreathPoint{1790, 129.87},
	  BreathPoint{1800, 129.80}, BreathPoint{1810, 129.71}, BreathPoint{1820, 129.61}, BreathPoint{1830, 129.49}, BreathPoint{1840, 129.35}, BreathPoint{1850, 129.20}, BreathPoint{1860, 129.03}, BreathPoint{1870, 128.84}, BreathPoint{1880, 128.64}, BreathPoint{1890, 128.43},
	  BreathPoint{1900, 128.20}, BreathPoint{1910, 127.95}, BreathPoint{1920, 127.69}, BreathPoint{1930, 127.41}, BreathPoint{1940, 127.12}, BreathPoint{1950, 126.81}, BreathPoint{1960, 126.49}, BreathPoint{1970, 126.15}, BreathPoint{1980, 125.80}, BreathPoint{1990, 125.43},
	  BreathPoint{2000, 125.05}, BreathPoint{2010, 124.65}, BreathPoint{2020, 124.24}, BreathPoint{2030, 123.82}, BreathPoint{2040, 123.38}, BreathPoint{2050, 122.92}, BreathPoint{2060, 122.46}, BreathPoint{2070, 121.97}, BreathPoint{2080, 121.48}, BreathPoint{2090, 120.97},
	  BreathPoint{2100, 120.45}, BreathPoint{2110, 119.92}, BreathPoint{2120, 119.37}, BreathPoint{2130, 118.81}, BreathPoint{2140, 118.24}, BreathPoint{2150, 117.65}, BreathPoint{2160, 117.06}, BreathPoint{2170, 116.45}, BreathPoint{2180, 115.83}, BreathPoint{2190, 115.20},
	  BreathPoint{2200, 114.55}, BreathPoint{2210, 113.90}, BreathPoint{2220, 113.23}, BreathPoint{2230, 112.56}, BreathPoint{2240, 111.87}, BreathPoint{2250, 111.17}, BreathPoint{2260, 110.47}, BreathPoint{2270, 109.75}, BreathPoint{2280, 109.02}, BreathPoint{2290, 108.29},
	  BreathPoint{2300, 107.54}, BreathPoint{2310, 106.79}, BreathPoint{2320, 106.03}, BreathPoint{2330, 105.26}, BreathPoint{2340, 104.48}, BreathPoint{2350, 103.69}, BreathPoint{2360, 102.90}, BreathPoint{2370, 102.10}, BreathPoint{2380, 101.29}, BreathPoint{2390, 100.47},
	  BreathPoint{2400,  99.65}, BreathPoint{2410,  98.82}, BreathPoint{2420,  97.99}, BreathPoint{2430,  97.15}, BreathPoint{2440,  96.30}, BreathPoint{2450,  95.45}, BreathPoint{2460,  94.59}, BreathPoint{2470,  93.73}, BreathPoint{2480,  92.87}, BreathPoint{2490,  92.00},
	  BreathPoint{2500,  91.13}, BreathPoint{2510,  90.25}, BreathPoint{2520,  89.37}, BreathPoint{2530,  88.49}, BreathPoint{2540,  87.60}, BreathPoint{2550,  86.71}, BreathPoint{2560,  85.82}, BreathPoint{2570,  84.93}, BreathPoint{2580,  84.03}, BreathPoint{2590,  83.14},
	  BreathPoint{2600,  82.24}, BreathPoint{2610,  81.35}, BreathPoint{2620,  80.45}, BreathPoint{2630,  79.55}, BreathPoint{2640,  78.65}, BreathPoint{2650,  77.76}, BreathPoint{2660,  76.86}, BreathPoint{2670,  75.96}, BreathPoint{2680,  75.07}, BreathPoint{2690,  74.18},
	  BreathPoint{2700,  73.29}, BreathPoint{2710,  72.40}, BreathPoint{2720,  71.51}, BreathPoint{2730,  70.63}, BreathPoint{2740,  69.75}, BreathPoint{2750,  68.87}, BreathPoint{2760,  68.00}, BreathPoint{2770,  67.13}, BreathPoint{2780,  66.27}, BreathPoint{2790,  65.41},
	  BreathPoint{2800,  64.55}, BreathPoint{2810,  63.70}, BreathPoint{2820,  62.85}, BreathPoint{2830,  62.01}, BreathPoint{2840,  61.18}, BreathPoint{2850,  60.35}, BreathPoint{2860,  59.53}, BreathPoint{2870,  58.71}, BreathPoint{2880,  57.90}, BreathPoint{2890,  57.10},
	  BreathPoint{2900,  56.31}, BreathPoint{2910,  55.52}, BreathPoint{2920,  54.74}, BreathPoint{2930,  53.97}, BreathPoint{2940,  53.21}, BreathPoint{2950,  52.45}, BreathPoint{2960,  51.71}, BreathPoint{2970,  50.97}, BreathPoint{2980,  50.25}, BreathPoint{2990,  49.53},
	  BreathPoint{3000,  48.83}, BreathPoint{3010,  48.13}, BreathPoint{3020,  47.44}, BreathPoint{3030,  46.77}, BreathPoint{3040,  46.10}, BreathPoint{3050,  45.45}, BreathPoint{3060,  44.80}, BreathPoint{3070,  44.17}, BreathPoint{3080,  43.55}, BreathPoint{3090,  42.94},
	  BreathPoint{3100,  42.35}, BreathPoint{3110,  41.76}, BreathPoint{3120,  41.19}, BreathPoint{3130,  40.63}, BreathPoint{3140,  40.08}, BreathPoint{3150,  39.55}, BreathPoint{3160,  39.03}, BreathPoint{3170,  38.52}, BreathPoint{3180,  38.03}, BreathPoint{3190,  37.54},
	  BreathPoint{3200,  37.08}, BreathPoint{3210,  36.62}, BreathPoint{3220,  36.18}, BreathPoint{3230,  35.76}, BreathPoint{3240,  35.35}, BreathPoint{3250,  34.95}, BreathPoint{3260,  34.57}, BreathPoint{3270,  34.20}, BreathPoint{3280,  33.85}, BreathPoint{3290,  33.51},
	  BreathPoint{3300,  33.19}, BreathPoint{3310,  32.88}, BreathPoint{3320,  32.59}, BreathPoint{3330,  32.31}, BreathPoint{3340,  32.05}, BreathPoint{3350,  31.80}, BreathPoint{3360,  31.57}, BreathPoint{3370,  31.36}, BreathPoint{3380,  31.16}, BreathPoint{3390,  30.97},
	  BreathPoint{3400,  30.80}, BreathPoint{3410,  30.65}, BreathPoint{3420,  30.51}, BreathPoint{3430,  30.39}, BreathPoint{3440,  30.29}, BreathPoint{3450,  30.20}, BreathPoint{3460,  30.13}, BreathPoint{3470,  30.07}, BreathPoint{3480,  30.03}, BreathPoint{3490,  30.01},
	  BreathPoint{3500,  30.00},
	};
}

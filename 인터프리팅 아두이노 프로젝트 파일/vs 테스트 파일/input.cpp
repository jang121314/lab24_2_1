

#include <chrono>
#include <thread>
#include "input.h"
#include "SoftwareSerial.h"

const char SoftwareSerial::PREDEFINED_DATA[BUFFER_SIZE] =
{ 0, 0, 
6, 1, 0, 0, 0, 1, 
4, 3, 2, 1, 1, 1, 33, 
3, 5, 1, 100, 35, 
2, 6, 1, 3, 37,
3, 6, 6, 0, 33,
5, 6, 14, 40,
5, 5, 3, 0,
9,
4, 14,
111, 1,
5, 16, 3, 0,
0 };

void delay(unsigned long ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


void main()
{
	Main::setup();
	while (1)
	{
		Main::loop();
		//delay(1);
	}
}
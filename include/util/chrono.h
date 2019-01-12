#ifndef _CRONO_H
#define _CRONO_H

#include <chrono>

class Chrono
{
	std::chrono::system_clock::time_point startTime;
	std::chrono::system_clock::time_point endTime;
	std::chrono::duration<double> elapsedTime; // segundos

public:
	Chrono();
	void start(void);
	void stop(void);
	double value(void);
};

#endif
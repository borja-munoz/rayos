#include "chrono.h"

Chrono::Chrono()
{
}

void Chrono::start(void)
{
  this->startTime = std::chrono::system_clock::now();
}

void Chrono::stop(void)
{
  this->endTime = std::chrono::system_clock::now();

  this->elapsedTime = (this->endTime - this->startTime);
}

double Chrono::value(void)
{
	return(this->elapsedTime.count());
}


#include "rand.h"

//--------------------------------------------------------------------

RandomGenerator::RandomGenerator()
{
  this->seed = 12345678;
}


//--------------------------------------------------------------------

RandomGenerator::RandomGenerator(int seed)
{
  this->seed = seed;
}


//--------------------------------------------------------------------

int RandomGenerator::getInteger(int x, int y)
{
	return(0);
}


//--------------------------------------------------------------------

double RandomGenerator::getReal(double x, double y)
{
	return(0.0);
}


//--------------------------------------------------------------------


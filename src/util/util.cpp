#include "util.h"

//--------------------------------------------------------------------

void initGenMT(unsigned long seed)
{
	init_genrand(seed);
}


//--------------------------------------------------------------------

void initGenSSE(unsigned int seed)
{
	srand_sse(seed);
}


//--------------------------------------------------------------------

real getRandomNumberMT(real x, real y)
{
	real result;
	
	result = (real) genrand_real1();

	return(x + ((y - x) * result));
}


//--------------------------------------------------------------------

unsigned long getRandomIntMT(int x, int y)
{
	double resultReal;

	resultReal = genrand_real1();

	return((unsigned long) floor(x + ((y - x) * resultReal) + 0.5));
}


//--------------------------------------------------------------------

real getRandomNumberSSE(real x, real y)
{
	real result;

    result = rand_sse() * INV_MAX_INT; 

	return(x + ((y - x) * result));
}


//--------------------------------------------------------------------

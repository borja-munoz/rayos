#ifndef _UTIL_H
#define _UTIL_H

#include <math.h>


//-------------------------------------------------------------------
// Numeric precision
typedef double real;
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Mersenne-Twister Random Number Generator functions
// (defined in file mt19937ar.cpp)
//-------------------------------------------------------------------
void init_genrand(unsigned long s);       
unsigned long genrand_int32(void);        
double genrand_real1(void);               

//-------------------------------------------------------------------
// Intel-SSE Random Number Generator functions
// (defined inf ile rndGenSSE.cpp)
//-------------------------------------------------------------------
void srand_sse(unsigned int s);           
unsigned int rand_sse(void);           

// Random number generator functions
void initGenMT(unsigned long seed);
void initGenSSE(unsigned int seed);
real getRandomNumberMT(real x, real y);
unsigned long getRandomIntMT(int x, int y);
real getRandomNumberSSE(real x, real y);


//-------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------

// PI
#define PI_RAYOS 3.141592                                 

// Convert degrees to radians
#define RADIANS(alfa) alfa * PI_RAYOS / 180.0            

// Returns 1 (true) if number is close to 0
#define ZERO(x) (fabs(x) < 0.0000001) ? 1 : 0       

// Constant for 1 / MaxInt (1.0 / 4294967295.0)
#define INV_MAX_INT 2.328306437E-10

// Memory alignment
#define ALIGN(limite) __declspec(align(limite))

#endif

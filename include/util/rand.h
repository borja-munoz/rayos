/*------------------------------------------------------------------*/
/* DESCRIPCI�N : Clase para representar un generador de n�meros     */
/*               aleatorios (Mersenne-Twister)                      */
/* FECHA       : 31/01/2004                                         */
/* AUTOR       : Borja Mu�oz Aguilar                                */
/*------------------------------------------------------------------*/

#ifndef _RAND_H
#define _RAND_H


//--------------------------------------------------------------------

class RandomGenerator
{
	int seed;

 public:
 
	RandomGenerator();
	RandomGenerator(int seed);
	int getInteger(int x, int y);
	double getReal(double x, double y);
};

#endif

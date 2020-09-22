#include "color.h"

Color::Color()
{
	this->r = 1.0;
	this->g = 1.0;
	this->b = 1.0;
}

Color::Color(real r, real g, real b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

real Color::getR(void)
{
    return(this->r);
}

real Color::getG(void)
{
    return(this->g);
}

real Color::getB(void)
{
    return(this->b);
}


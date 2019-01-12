#include "light.h"

//-------------------------------------------------------------------------------

Light::Light()
{
	this->color[0] = 1.0;
	this->color[1] = 1.0;
	this->color[2] = 1.0;
	this->intensity = 1.0;
}


//-------------------------------------------------------------------------------

Light::Light(real *color, real intensity)
{
	for (int i = 0; i < 3; i++)
		this->color[i] = color[i];
	this->intensity = intensity;
}

//-------------------------------------------------------------------------------

Light::Light(Light &l)
{
	for (int i = 0; i < 3; i++)
		this->color[i] = l.color[i];
	this->intensity = l.intensity;
}


//-------------------------------------------------------------------------------

Light::~Light()
{

}


//-------------------------------------------------------------------------------

real * Light::getColor(void)
{
	real *color;
	  
	color = new real[3];
	for (int i = 0; i < 3; i++)
		color[i] = this->color[i];
	    
	return(color);
}


//-------------------------------------------------------------------------------

real Light::getIntensity(void)
{
	return(this->intensity);
}


//-------------------------------------------------------------------------------

void Light::setColor(real *color)
{
	for (int i = 0; i < 3; i++)
		this->color[i] = color[i];
}


//-------------------------------------------------------------------------------

void Light::setIntensity(real intensity)
{
	this->intensity = intensity;
}


//-------------------------------------------------------------------------------


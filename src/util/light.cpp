#include "light.h"

Light::Light()
{
    this->color = Color();
	this->intensity = 1.0;
}

Light::Light(Color color, real intensity)
{
    this->color = color;
	this->intensity = intensity;
}

const Color& Light::getColor(void) const
{
	return(this->color);
}

real Light::getIntensity(void) const
{
	return(this->intensity);
}

void Light::setColor(Color color)
{
    this->color = color;
}

void Light::setIntensity(real intensity)
{
	this->intensity = intensity;
}



#include "light.h"

Light::Light()
{
    this->color = std::make_shared<Color>();
	this->intensity = 1.0;
}

Light::Light(std::shared_ptr<Color> color, real intensity)
{
    this->color = color;
	this->intensity = intensity;
}

std::shared_ptr<Color> Light::getColor(void)
{
	return(this->color);
}

real Light::getIntensity(void)
{
	return(this->intensity);
}

void Light::setColor(std::shared_ptr<Color> color)
{
    this->color = color;
}

void Light::setIntensity(real intensity)
{
	this->intensity = intensity;
}



#include "material.h"


//-----------------------------------------------

Material::Material()
{
    this->color = std::make_shared<Color>(1.0, 1.0, 1.0);
	this->ka = 1.0;
	this->kd = 0.5; 
	this->ks = 0.5;
    this->ke = 0.0;
}

std::shared_ptr<Color> Material::getColor(void)
{
    return(color);
}

real Material::getKa(void)
{
    return(this->ka);
}

real Material::getKd(void)
{
    return(this->kd);
}

real Material::getKs(void)
{
    return(this->ks);
}

real Material::getKe(void)
{
    return(this->ke);
}

void Material::set(std::shared_ptr<Color> color, real ka, real kd, real ks, real ke)
{
    this->color = color;
  
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->ke = ke;
}


//-----------------------------------------------


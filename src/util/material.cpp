#include "material.h"


//-----------------------------------------------

Material::Material()
{
	this->color[0] = 1.0;
	this->color[1] = 1.0;
	this->color[2] = 1.0;
	this->ka = 1.0;
	this->kd = 0.5; 
	this->ks = 0.5;
    this->ke = 0.0;
}


//-----------------------------------------------

Material::Material(const Material &m)
{
	for (int i = 0; i < 3; i++)
		this->color[i] = m.color[i];
	this->ka = m.ka;
	this->kd = m.kd;
	this->ks = m.ks;
    this->ke = m.ke;
}


//-----------------------------------------------

Material::~Material()
{
	
}


//-----------------------------------------------

real * Material::getColor(void)
{
  real *color;
  
  color = new real[3];
  for (int i = 0; i < 3; i++)
    color[i] = this->color[i];
    
  return(color);
}


//-----------------------------------------------

real Material::getKa(void)
{
  return(this->ka);
}

//-----------------------------------------------

real Material::getKd(void)
{
  return(this->kd);
}

//-----------------------------------------------

real Material::getKs(void)
{
  return(this->ks);
}

//-----------------------------------------------

real Material::getKe(void)
{
  return(this->ke);
}

//-----------------------------------------------

void Material::set(real *color, real ka, real kd, real ks, real ke)
{
  for (int i = 0; i < 3; i++)
    this->color[i] = color[i];
  
  this->ka = ka;
  this->kd = kd;
  this->ks = ks;
  this->ke = ke;
}


//-----------------------------------------------


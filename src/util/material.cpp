#include "material.h"

const Color& Material::getColor(void) const
{
    return(this->kdColor);
}

real Material::getKa(void) const
{
    return(this->ka);
}

real Material::getKd(void) const
{
    return(this->kd);
}

real Material::getKs(void) const
{
    return(this->ks);
}

real Material::getKe(void) const
{
    return(this->ke);
}

void Material::set(Color color, real ka, real kd, real ks, real ke)
{
    this->kdColor = color;
  
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->ke = ke;
}


//-----------------------------------------------


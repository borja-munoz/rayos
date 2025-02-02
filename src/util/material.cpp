#include "material.h"

Color Material::getColor(void)
{
    return(kdColor);
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

void Material::set(Color color, real ka, real kd, real ks, real ke)
{
    this->kdColor = color;
  
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->ke = ke;
}


//-----------------------------------------------


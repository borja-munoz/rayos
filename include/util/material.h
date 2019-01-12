#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "util.h"

class Material
{
  real color[3];            
  real ka;                  // Ambient
  real kd;                  // Diffuse
  real ks;                  // Specular
  
 public:
 
  Material();
  Material(const Material &m);
  ~Material();
  real *getColor(void);
  real getKa(void);
  real getKd(void);
  real getKs(void);
  void set(real *color, real ka, real kd, real ks);
};


#endif

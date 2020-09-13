#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "util.h"

class Material
{
  real color[3];            
  real ka;                  // Ambient
  real kd;                  // Diffuse
  real ks;                  // Specular
  real ke;                  // Emissive (for light sources)
  
 public:
 
  Material();
  Material(const Material &m);
  ~Material();
  real *getColor(void);
  real getKa(void);
  real getKd(void);
  real getKs(void);
  real getKe(void);
  void set(real *color, real ka, real kd, real ks, real ke);
};


#endif

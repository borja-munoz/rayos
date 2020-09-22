#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "color.h"
#include "util.h"

#include <memory>

class Material
{
    std::shared_ptr<Color> color;            
    real ka;   // Ambient
    real kd;   // Diffuse
    real ks;   // Specular
    real ke;   // Emissive (for light sources)
  
  public:
 
    Material();
    std::shared_ptr<Color> getColor(void);
    real getKa(void);
    real getKd(void);
    real getKs(void);
    real getKe(void);
    void set(std::shared_ptr<Color> color, real ka, real kd, real ks, real ke);
};


#endif

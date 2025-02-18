#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "color.h"
#include "util.h"

#include <memory>

class Material
{
    Color kdColor;  // Diffuse color (can be RGB or spectrum)
    real ka;   // Ambient
    real kd;   // Diffuse
    real ks;   // Specular
    real ke;   // Emissive (for light sources)
  
  public:
 
    Material() : kdColor(Color(1.0, 1.0, 1.0)), ka(1.0f), kd(0.5f), ks(0.5f), ke(0.0f) {}
    Material(const Color& color, real ka, real kd, real ks, real ke)
        : kdColor(color), ka(ka), kd(kd), ks(ks), ke(ke) {} 
    const Color& getColor(void) const;
    real getKa(void) const;
    real getKd(void) const;
    real getKs(void) const;
    real getKe(void) const;
    void set(Color color, real ka, real kd, real ks, real ke);
};


#endif

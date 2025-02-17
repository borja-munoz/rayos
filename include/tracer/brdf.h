#ifndef _BRDF_H
#define _BRDF_H

#include "../util/light.h"
#include "../util/material.h"

#include <memory>

// Types
typedef enum
{
	PHONG,
	BLINN,
	COOK_TORRANCE,
	HE,
	LAFORTUNE
} BRDFtype;

// Prototypes
Color PhongBRDF(Material m, 
                Vector3D N, 
                std::shared_ptr<Light> light,
                Vector3D L, 
                Vector3D V);
Color BlinnBRDF(Material m, 
                Vector3D N, 
                std::shared_ptr<Light> light,
                Vector3D L, 
                Vector3D V);

#endif

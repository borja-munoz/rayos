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
Color PhongBRDF(std::shared_ptr<Material> m, 
                Vector3D N, 
                std::shared_ptr<Vector3D> L, 
                std::shared_ptr<Vector3D> V);
Color BlinnBRDF(std::shared_ptr<Material> m, 
                Vector3D N, 
                std::shared_ptr<Vector3D> L, 
                std::shared_ptr<Vector3D> V);

#endif

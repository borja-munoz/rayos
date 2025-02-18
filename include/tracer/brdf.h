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
Color PhongBRDF(const Material& m, 
                const Vector3D& N, 
                const Light& light,
                const Vector3D& L, 
                const Vector3D& V);
Color BlinnBRDF(const Material& m, 
                const Vector3D& N, 
                const Light& light,
                const Vector3D& L, 
                const Vector3D& V);

#endif

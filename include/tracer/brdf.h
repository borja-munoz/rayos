#ifndef _BRDF_H
#define _BRDF_H

#include "../util/light.h"
#include "../util/material.h"

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
real *PhongBRDF(Material *m, Vector3D N, Vector3D *L, Vector3D *V);
real *BlinnBRDF(Material *m, Vector3D N, Vector3D *L, Vector3D *V);

#endif

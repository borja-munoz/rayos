#include "brdf.h"

/*--------------------------------------------------------*/
/* Bui T. Phong BRDF                                      */
/*--------------------------------------------------------*/
real *PhongBRDF(Material *mat, Vector3D N, Vector3D *L, Vector3D *V)
{
	Vector3D *R, *aux;
	real ka, kd, ks;
	real *colorLight, intensityLight, *colorMat;
	double NL, RV;
	real *radiance = new real[3];

	// Ambient, diffuse and specular components
	ka = mat->getKa();
	kd = mat->getKd();
	ks = mat->getKs();

	colorMat = mat->getColor();

	colorLight = new real[3];
	colorLight[0] = colorLight[1] = colorLight[2] = 1.0f;
	intensityLight = 1.0f;   

	// Vector for diffuse component
    NL = N.dotProduct(L);
    
    // Vectors for specular component
    aux = N.product(2 * NL);
    R = aux->substract(L);    
    RV = R->dotProduct(V);
    if (RV < 0.0)
		RV = 0.0;
    else
        RV = (real) pow(RV, 0.5);     // RV^0.5
	delete(R);
	delete(aux);

    if (NL < 0.0)
        NL = 0.0;

    // We calculate the final color taking into account both diffuse and specular components
    radiance[0] = (colorLight[0] * intensityLight * colorMat[0]) * 
                   (kd * NL + ks * RV);
    radiance[1] = (colorLight[1] * intensityLight * colorMat[1]) * 
                   (kd * NL + ks * RV);
    radiance[2] = (colorLight[2] * intensityLight * colorMat[2]) *
                   (kd * NL + ks * RV);

	delete[](colorLight);
	delete[](colorMat);

	return(radiance);
}


/*--------------------------------------------------------*/

/*--------------------------------------------------------*/
/* Jim Blinn BRDF                                         */
/*--------------------------------------------------------*/
real *BlinnBRDF(Material *m, Vector3D N, Vector3D *L, Vector3D *V)
{
	real *radiance = new real[3];

	// Not finished, only to avoid warnings
	radiance[0] = N.dotProduct(L) + L->dotProduct(V) + m->getKa();

	return(radiance);
}


/*--------------------------------------------------------*/

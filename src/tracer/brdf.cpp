#include "brdf.h"

// Bui T. Phong BRDF                                      
Color PhongBRDF(Material mat, 
                Vector3D N, 
                std::shared_ptr<Light> light,
                Vector3D L, 
                Vector3D V)
{
    Vector3D R, aux;
	  real ka, kd, ks;
    Color colorMat;
	  double NL, RV;
	  Color radiance;

    // Ambient, diffuse and specular components
    ka = mat.getKa();
    kd = mat.getKd();
    ks = mat.getKs();

    colorMat = mat.getColor();
    
	  // Vector for diffuse component
    NL = N.dotProduct(L);
    
    // Vectors for specular component
    aux = N.product(2 * NL);
    R = aux.substract(L);    
    RV = R.dotProduct(V);
    if (RV < 0.0)
		RV = 0.0;
    else
        RV = (real) pow(RV, 0.5);     // RV^0.5

    if (NL < 0.0)
        NL = 0.0;

    // We calculate the final color taking into account both diffuse and specular components
    /*
    radiance = Color((colorLight.getR() * intensityLight * colorMat.getR()) * (kd * NL + ks * RV),
                     (colorLight.getG() * intensityLight * colorMat.getG()) * (kd * NL + ks * RV),
                     (colorLight.getB() * intensityLight * colorMat.getB()) * (kd * NL + ks * RV));
    */
    radiance = light->getColor() * 
               light->getIntensity() * 
               colorMat * 
               (kd * NL + ks * RV);

	  return(radiance);
}

// Jim Blinn BRDF                                         
Color BlinnBRDF(Material mat, 
                Vector3D N, 
                std::shared_ptr<Light> light,
                Vector3D L, 
                Vector3D V)
{
    Color radiance;

    // Not finished, only to avoid warnings
    //radiance[0] = N.dotProduct(L) + L->dotProduct(V) + mat->getKa();
      radiance = Color();

    return(radiance);
}

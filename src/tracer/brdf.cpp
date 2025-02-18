#include "brdf.h"

// Bui T. Phong BRDF                                      
Color PhongBRDF(const Material& mat, 
                const Vector3D& N, 
                const Light& light,
                const Vector3D& L, 
                const Vector3D& V)
{
    // Ambient, diffuse and specular components
    real ka = mat.getKa();
    real kd = mat.getKd();
    real ks = mat.getKs();

    // Use reference to avoid deep copy
    const Color &colorMat = mat.getColor();
    
	  // Calculate diffuse component
    real NL = N.dotProduct(L);
    if (NL < 0.0) 
      NL = 0.0;

    // Calculate specular component
    Vector3D R = (N * (2 * NL)) - L;   // Inline computation
    real RV = R.dotProduct(V);
    if (RV < 0.0) 
      RV = 0.0;
    else 
      RV = static_cast<real>(pow(RV, 0.5)); // Avoid extra copy

    if (NL < 0.0)
      NL = 0.0;

    // Calculate the final color with the diffuse and specular components
    return light.getColor() * 
           light.getIntensity() * 
           colorMat * 
           (kd * NL + ks * RV);
}

// Jim Blinn BRDF                                         
Color BlinnBRDF(const Material& mat, 
                const Vector3D& N, 
                const Light& light,
                const Vector3D& L, 
                const Vector3D& V)
{
    Color radiance;

    // Not finished, only to avoid warnings
    //radiance[0] = N.dotProduct(L) + L->dotProduct(V) + mat->getKa();
      radiance = Color();

    return(radiance);
}

#ifndef _CAMERA_H
#define _CAMERA_H

#include "point3D.h"
#include "vector3D.h"
#include "ray.h"
#include "util.h"

#include <math.h>

#include <vector>

using namespace std;

//--------------------------------------------------------------------------------------------

class Camera
{
	Point3D *location;                              // Camera location
	Point3D *view;                                  // Look-At Point
	Vector3D *up;                                   // Fija la orientaci�n
	real distance;                                  // Distance to viewplane
	real hFov, vFov;                                // Horizontal and Vertical Field of View
	unsigned int xRes, yRes;                        // Viewplane resolution (image size)
	Vector3D *xCam, *yCam, *zCam;                   // Camera coordinate system
	Point3D *oViewport;                             // Viewplane viewport origin
	Vector3D *uViewport, *vViewport;                // Viewplane viewport axes
	real pixelWidth, pixelHeight;                     // Pixel dimensions

	void initialize(void);

  public:
 
	Camera();
	Camera(Point3D *location, Point3D *view, Vector3D *up, real distance, 
		   real hFov, real vFov, unsigned int xRes, unsigned int yRes);
	Camera(Camera &);
	~Camera();
	Point3D *getLocation(void);
	void getResolution(unsigned int &xRes, unsigned int &yRes);
	Ray *getEyeRay(real x, real y);
	vector<Ray *> *getSampleEyeRays(real x, real y, unsigned int sampleRays);

};


//--------------------------------------------------------------------------------------------

#endif

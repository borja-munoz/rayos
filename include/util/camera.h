#ifndef _CAMERA_H
#define _CAMERA_H

#include "point3D.h"
#include "vector3D.h"
#include "ray.h"
#include "util.h"

#include <math.h>

#include <memory>
#include <vector>

using namespace std;

class Camera
{
	std::shared_ptr<Point3D> location;              // Camera location
	std::shared_ptr<Point3D> view;                  // Look-At Point
	std::shared_ptr<Vector3D> up;                   // Orientation
	real distance;                                  // Distance to viewplane
	real hFov, vFov;                                // Horizontal and Vertical Field of View
	unsigned int xRes, yRes;                        // Viewplane resolution (image size)
	std::shared_ptr<Vector3D> xCam, yCam, zCam;     // Camera coordinate system
	std::shared_ptr<Point3D> oViewport;             // Viewplane viewport origin
	std::shared_ptr<Vector3D> uViewport, vViewport; // Viewplane viewport axes
	real pixelWidth, pixelHeight;                   // Pixel dimensions

	void initialize(void);

  public:
 
	Camera();
	Camera(std::shared_ptr<Point3D> location, 
           std::shared_ptr<Point3D> view, 
           std::shared_ptr<Vector3D> up, 
           real distance, 
		   real hFov, 
           real vFov, 
           unsigned int xRes, 
           unsigned int yRes);
	std::shared_ptr<Point3D> getLocation(void);
	void getResolution(unsigned int &xRes, unsigned int &yRes);
	std::shared_ptr<Ray> getEyeRay(real x, real y);
	vector<std::shared_ptr<Ray>> getSampleEyeRays(real x, real y, unsigned int sampleRays);

};

#endif

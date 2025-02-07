#include "camera.h"


Camera::Camera()
{
	this->location = Point3D(0, 0, -0.75);
	this->view = Point3D(0, 0, -2);
	this->up = Vector3D(0, 1, 0);
	this->hFov = 60;
	this->vFov = 60;
	this->distance = 1;
	this->xRes = this->yRes = 640;

	this->initialize();
}

Camera::Camera(Point3D location, 
               Point3D view, 
               Vector3D up)
{
	this->location = location;
	this->view = view;
	this->up = up;
	this->distance = 1;
	this->hFov = 60;
	this->vFov = 60;
	this->xRes = 640;
	this->yRes = 640;

	this->initialize();	
}


Camera::Camera(Point3D location, 
               Point3D view, 
               Vector3D up, 
               real distance, 
	           real hFov, 
               real vFov, 
               unsigned int xRes, 
               unsigned int yRes)
{
	this->location = location;
	this->view = view;
	this->up = up;
	this->distance = distance;
	this->hFov = hFov;
	this->vFov = vFov;
	this->xRes = xRes;
	this->yRes = yRes;

	this->initialize();
}

void Camera::initialize(void)
{
	// Camera coordinate system
	this->zCam = this->view.substract(this->location);          // Z axis is the direction from the camera to the point where the
    this->zCam.normalize();                                     // camera is pointing
	this->xCam = this->up.crossProduct(this->zCam);             // X axis is perpendicular to Z axis and UP vector
	this->xCam.normalize();
	this->yCam = this->xCam.crossProduct(this->zCam);           // Y axis is perpendicular to X and Z axis
	this->yCam.normalize();

	// Viewport origin (Camera location shifted in a distance equal to the viewplane in 
    // the direction the camera is pointing (Z axis)
	this->oViewport = this->location.sum(this->zCam.product(this->distance));	

	// Viewport axis in the viewplane
    this->uViewport = this->xCam.product(this->distance * tan(RADIANS(this->hFov) / 2.0));
    this->vViewport = this->yCam.product(this->distance * tan(RADIANS(this->vFov) / 2.0));
}

Point3D Camera::getLocation(void)
{
	return(this->location);
}

void Camera::getResolution(unsigned int &xRes, unsigned int &yRes)
{
	xRes = this->xRes;
	yRes = this->yRes;
}

void Camera::setFOV(real fov)
{
	this->hFov = fov;
	this->vFov = fov;
	this->initialize();
}

Ray Camera::getEyeRay(real x, real y)
{
	Vector3D direction;
	real alfa, beta;
	Point3D pViewport;

    // First we need to calculate the point in the viewport where the ray goes through
    // Compute normalized device coordinates
	// P = oViewport + (alfa * uViewport) + (beta * vViewport)
	// alfa y beta represents the pixel center
	alfa = ((2 * (x + 0.5)) / this->xRes) - 1; // Range [-1, 1]
	beta = 1 - ((2 * (y + 0.5)) / this->yRes); // Inverted because Y axis in the viewport is different than the one in the image

	// Compute the point on the viewport in world space
    pViewport = oViewport
				.sum(uViewport.product(alfa))  // Horizontal displacement
				.sum(vViewport.product(beta)); // Vertical displacement

    // Ray direction will be the vector from the point in the viewport to the camera location
	direction = pViewport.substract(this->location);
	direction.normalize();

	// We use the camera location as ray origin
	return Ray(this->location, direction);
}

vector<Ray> Camera::getSampleEyeRays(real x, real y, unsigned int sampleRays)
{
	Vector3D direction;
	real alfa, beta;
	Point3D pViewport;
	vector<Ray> eyeRays;
	real jitterX, jitterY;
	real sampleRaysDiv2;

    // This value will be used many times so we precalculate it
	sampleRaysDiv2 = sampleRays / 2.0;

	for (int i = 0; i < sampleRaysDiv2; i++)
		for (int j = 0; j < sampleRaysDiv2; j++)
		{
            // We jitter the alfa and beta parameters to improve point sampling     

			// Jitter variable contains a value within each of the regions
            // that form the pixel (the pixel is divided in as many regions as sampleRays)
			jitterX = getRandomNumberMT(i / sampleRaysDiv2, (i + 1) / sampleRaysDiv2);
			jitterY = getRandomNumberMT(j / sampleRaysDiv2, (j + 1) / sampleRaysDiv2);

			alfa = ((2 * (x + jitterX)) / this->xRes) - 1;
			
            // Inverted because Y axis is different in the viewport and the image
			beta = 1 - ((2 * (y + jitterY)) / this->yRes); 

			// Finally we can get the point in the viewport
			Vector3D uAlfa = uViewport.product(alfa);
			Vector3D vBeta = vViewport.product(beta);
			Vector3D desp = uAlfa.sum(vBeta);
			pViewport = oViewport.sum(desp);

            // Ray direction will be the vector from the viewport point to the camera location
			direction = pViewport.substract(this->location);
			direction.normalize();

			// We crete the ray with the camera location as origin
			eyeRays.push_back(Ray(this->location, direction));
		}

	return(eyeRays);
}


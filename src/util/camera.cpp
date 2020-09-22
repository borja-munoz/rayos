#include "camera.h"


Camera::Camera()
{
	this->location = std::make_shared<Point3D>(0, 0, -0.75);
	this->view = std::make_shared<Point3D>(0, 0, -2);
	this->up = std::make_shared<Vector3D>(0, 1, 0);
	this->hFov = 60;
	this->vFov = 60;
	this->distance = 1;
	this->xRes = this->yRes = 600;

	this->initialize();
}


Camera::Camera(std::shared_ptr<Point3D> location, 
               std::shared_ptr<Point3D> view, 
               std::shared_ptr<Vector3D> up, 
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
	this->zCam = this->view->substract(this->location);          // Z axis is the direction from the camera to the point where the
                                                                 // camera is pointing
	this->xCam = this->zCam->crossProduct(this->up);             // X axis is perpendicular to Z axis and UP vector
	this->xCam->normalize();
	this->yCam = this->zCam->crossProduct(this->xCam);           // Y axis is perpendicular to X and Z axis
	this->yCam->normalize();

	// Viewport origin (Camera location shifted in a distance equal to the viewplane in 
    // the direction the camera is pointing (Z axis)
	this->oViewport = this->location->sum(this->zCam->product(this->distance));	

	// Viewport axis in the viewplane
	this->uViewport = this->xCam->product(this->distance * tan(RADIANS(this->hFov)));
	this->vViewport = this->yCam->product(this->distance * tan(RADIANS(this->vFov)));
}

std::shared_ptr<Point3D> Camera::getLocation(void)
{
	return(std::make_shared<Point3D>(*(this->location)));
}

void Camera::getResolution(unsigned int &xRes, unsigned int &yRes)
{
	xRes = this->xRes;
	yRes = this->yRes;
}

std::shared_ptr<Ray> Camera::getEyeRay(real x, real y)
{
	std::shared_ptr<Vector3D> direction;
	std::shared_ptr<Ray> eyeRay;
	real alfa, beta;
	std::shared_ptr<Point3D> pViewport;

    // First we need to calculate the point in the viewport where the ray goes through
	// P = oViewport + (alfa * uViewport) + (beta * vViewport)
	// alfa y beta represents the pixel center
	alfa = ((2 * (x + 0.5)) / this->xRes) - 1;
	beta = 1 - ((2 * (y + 0.5)) / this->yRes); // Inverted because Y axis in the viewport is different than the one in the image
	pViewport = oViewport->sum(uViewport->product(alfa)->sum(vViewport->product(beta)));

    // Ray direction will be the vector from the point in the viewport to the camera location
	direction = pViewport->substract(this->location);
	direction->normalize();

	// We use the camera location as ray origin
	eyeRay = std::make_shared<Ray>(this->location, direction);

	return(eyeRay);
}


vector<std::shared_ptr<Ray>> Camera::getSampleEyeRays(real x, real y, unsigned int sampleRays)
{
	std::shared_ptr<Vector3D> direction;
	std::shared_ptr<Ray> eyeRay;
	real alfa, beta;
	std::shared_ptr<Point3D> pViewport;
	vector<std::shared_ptr<Ray>> eyeRays;
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
			std::shared_ptr<Vector3D> uAlfa = uViewport->product(alfa);
			std::shared_ptr<Vector3D> vBeta = vViewport->product(beta);
			std::shared_ptr<Vector3D> desp = uAlfa->sum(vBeta);
			pViewport = oViewport->sum(desp);

            // Ray direction will be the vector from the viewport point to the camera location
			direction = pViewport->substract(this->location);
			direction->normalize();

			// We crete the ray with the camera location as origin
			eyeRay = std::make_shared<Ray>();
			eyeRay->setOrigin(this->location);
			eyeRay->setDirection(direction);
			
			eyeRays.push_back(eyeRay);
		}

	return(eyeRays);
}


//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <iostream>
#include <ctime>   
#include <sstream>
#include <string>
#include <iomanip>

#include <stdio.h>
#include <math.h>

#include "util/chrono.h"
#include "scene.h"
#include "bitmap.h"
#include "tracer/whittedRayTracer.h"
#include "tracer/stochasticRayTracer.h"
#include "tracer/brdf.h"

//-----------------------------------------------

int main(int argc, char *argv[])
{
	Scene *e;
	Bitmap *im;
	Tracer *rt;

//	File3ds *f = new File3ds("knot.3ds");

	if (argc == 4)
	{
		// Iniciar cron�metro 
		Chrono *c = new Chrono();	
		c->start();

		// Crear scene
		e = new Scene();

		// Crear imagen 

		if (!strcmp(argv[1], "w"))
		{
			// Whitted RayTracer
			//WhittedRayTracer *wrt;
			rt = new WhittedRayTracer(PHONG);
			//wrt = new WhittedRayTracer();
			//im = wrt->trace(new Scene());
		}
		else
		{
			// Stochastic RayTracer
			//StochasticRayTracer *srt;
			//srt = new StochasticRayTracer();
			rt = new StochasticRayTracer(atoi(argv[2]), atoi(argv[3]));
		}

		im = rt->trace(e);

		c->stop();
		cout << "Elapsed time = " << c->value() << " seconds\n";
		delete(c);

		/*std::stringstream nombreFichero;
		std::time_t fechaActual = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		const char *formatoFecha = "%Y-%m-%d";
		std::stringstream fecha;
		fecha = std::put_time(fechaActual, formatoFecha);
		nombreFichero << "imagen" << fecha << ".tga";
		im->writeTGA(nombreFichero.str());  */
		im->writeTGA("imagen.tga");  

		delete(e);
		delete(im);

		// Windows
		//system("start imagen.tga");	
		
		// Mac OS X
		system("open imagen.tga");	
	}
	else
		cout << endl << "Usage: rayos [w|s] <sampleRays> <shadowRays>" << endl;

	return(0);
}


//-----------------------------------------------


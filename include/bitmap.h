#ifndef _BITMAP_H
#define _BITMAP_H

#include "util/util.h"

#include <string>

#include <stdio.h>

class Bitmap
{
	unsigned int width;
	unsigned int height;
	unsigned char *bufferRGB;
	real *bufferHDR;

  private:
	void toneMapper(void);

  public:

	Bitmap();
	Bitmap(unsigned int width, unsigned int height);
	~Bitmap();
	void setRGBPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);
	void setHDRPixel(unsigned int x, unsigned int y, real r, real g, real b);
	void writeTGA(const char *filename);

};


#endif
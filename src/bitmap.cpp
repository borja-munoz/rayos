#include "bitmap.h"

Bitmap::Bitmap()
{

}

Bitmap::Bitmap(unsigned int width, unsigned int height)
{
    this->width = width;
	this->height = height;
	this->bufferRGB = new unsigned char[this->width * this->height * 3];
	this->bufferHDR = new real[this->width * this->height * 3];
}

Bitmap::~Bitmap()
{
	delete bufferRGB;
	delete bufferHDR;
}

void Bitmap::setRGBPixel(unsigned int i, unsigned int j, unsigned char r, unsigned char g, unsigned char b)
{
    this->bufferRGB[this->width * i * 3 + j * 3    ] = r;
    this->bufferRGB[this->width * i * 3 + j * 3 + 1] = g;
    this->bufferRGB[this->width * i * 3 + j * 3 + 2] = b;
}


void Bitmap::setHDRPixel(unsigned int i, unsigned int j, Color color)
{
    this->bufferHDR[this->width * i * 3 + j * 3    ] = color.getR();
    this->bufferHDR[this->width * i * 3 + j * 3 + 1] = color.getG();
    this->bufferHDR[this->width * i * 3 + j * 3 + 2] = color.getB();
}


// Lineal tonemapper
void Bitmap::toneMapper(void)
{
	unsigned int i;
	real maxValue, minValue, value;
	real *ptrHDR;
	unsigned char *ptrRGB;

	ptrHDR = this->bufferHDR;
	maxValue = this->bufferHDR[0];
	minValue = this->bufferHDR[0];
	ptrHDR++;
	for (i = 1; i < (this->width * this->height * 3); i++)
	{
		value = *ptrHDR++;

		if (value > maxValue)
			maxValue = value;
		else if (value < minValue)
			minValue = value;
	}

	// Escalar al rango [0-255]
	ptrRGB = this->bufferRGB;
	ptrHDR = this->bufferHDR;
	for (i = 0; i < (this->width * this->height * 3); i++)	
		*ptrRGB++ = ((*ptrHDR++ - minValue) / (maxValue - minValue)) * 255;

}


void Bitmap::writeTGA(const char *nombreFichero)
{
	FILE *filename;

	// We use a tonemapper to go from HDR to RGB
	this->toneMapper();

	filename = fopen(nombreFichero, "wb");

	// TGA header
	fputc(0, filename);                            // No Identification Field
	fputc(0, filename);                            // No colormap 
	fputc(2, filename);                            // type 2: no compression RGB 
	for (int i = 0; i < 5; i++)
		fputc(0, filename);						   // No data for colormap
	fputc(0, filename);                            // Bottom left X coordinate
	fputc(0, filename);							   // expressed as an integer
	fputc((this->height - 1) & 0x00FF, filename);    // Bottom left Y coordinate
	fputc((this->height & 0xFF00) / 256, filename);  // expressed as an integer (Little-Endian)
	fputc(this->width & 0x00FF, filename);         // Image width
	fputc((this->width & 0xFF00) / 256, filename); // expressed as an integer (Little-Endian)
	fputc(this->height & 0x00FF, filename);          // Image height
	fputc((this->height & 0xFF00) / 256, filename);  // eexpressed as an integer (Little-Endian)
	fputc(24, filename);                           // 24 bits per pixel
	fputc(0, filename);                            // No image descriptor 
	//fputc(0, filename);
	//fputc(0, filename);
  
    // The pixels are written in BRR order
    // Photoshop does not take into account the vertical orientation parameter
    // so we must written it backwards
	for (int i = 0; i < this->height; i++)
		for (unsigned int j = 0; j < this->width; j++)
		{
			fputc(this->bufferRGB[this->width * i * 3 + j * 3 + 2], filename);
			fputc(this->bufferRGB[this->width * i * 3 + j * 3 + 1], filename);
			fputc(this->bufferRGB[this->width * i * 3 + j * 3    ], filename);
		}

	// RAW format
	/*for (unsigned int i = 0; i < this->height; i++)
		for (unsigned int j = 0; j < this->width; j++)
		{
			fputc(this->bufferRGB[this->width * i * 3 + j * 3    ], filename);
			fputc(this->bufferRGB[this->width * i * 3 + j * 3 + 1], filename);
			fputc(this->bufferRGB[this->width * i * 3 + j * 3 + 2], filename);
		}*/
    
	fclose(filename);
}
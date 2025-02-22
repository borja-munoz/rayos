#ifndef _COLOR_H
#define _COLOR_H

#include "util.h"

#include <iostream>
#include <map>

// The CIE 1931 color matching functions (x̅(λ), y̅(λ), z̅(λ)) 
// provide weights for converting spectral data to XYZ coordinates.
// These are the sample discrete values for wavelengths 
// from 380 nm to 780 nm in 5 nm steps:
const std::map<int, std::tuple<float, float, float>> CIE_XYZ = {
    {380, {0.0014f, 0.0000f, 0.0065f}}, {385, {0.0022f, 0.0001f, 0.0105f}},
    {390, {0.0042f, 0.0001f, 0.0201f}}, {395, {0.0076f, 0.0002f, 0.0362f}},
    {400, {0.0143f, 0.0004f, 0.0679f}}, {405, {0.0232f, 0.0006f, 0.1102f}},
    {410, {0.0435f, 0.0012f, 0.2074f}}, {415, {0.0776f, 0.0022f, 0.3713f}},
    {420, {0.1344f, 0.0040f, 0.6456f}}, {425, {0.2148f, 0.0073f, 1.0391f}},
    {430, {0.2839f, 0.0116f, 1.3856f}}, {435, {0.3285f, 0.0168f, 1.6229f}},
    {440, {0.3483f, 0.0230f, 1.7471f}}, {445, {0.3481f, 0.0298f, 1.7826f}},
    {450, {0.3362f, 0.0380f, 1.7721f}}, {455, {0.3187f, 0.0480f, 1.7441f}},
    {460, {0.2908f, 0.0600f, 1.6692f}}, {465, {0.2511f, 0.0739f, 1.5281f}},
    {470, {0.1954f, 0.0910f, 1.2876f}}, {475, {0.1421f, 0.1126f, 1.0419f}},
    {480, {0.0956f, 0.1390f, 0.8130f}}, {485, {0.0580f, 0.1693f, 0.6162f}},
    {490, {0.0320f, 0.2080f, 0.4652f}}, {495, {0.0147f, 0.2586f, 0.3533f}},
    {500, {0.0049f, 0.3230f, 0.2720f}}, {505, {0.0024f, 0.4073f, 0.2123f}},
    {510, {0.0093f, 0.5030f, 0.1582f}}, {515, {0.0291f, 0.6082f, 0.1117f}},
    {520, {0.0633f, 0.7100f, 0.0782f}}, {525, {0.1096f, 0.7932f, 0.0573f}},
    {530, {0.1655f, 0.8620f, 0.0422f}}, {535, {0.2257f, 0.9149f, 0.0298f}},
    {540, {0.2904f, 0.9540f, 0.0203f}}, {545, {0.3597f, 0.9803f, 0.0134f}},
    {550, {0.4334f, 0.9949f, 0.0087f}}, {555, {0.5121f, 1.0000f, 0.0057f}},
    {560, {0.5945f, 0.9950f, 0.0039f}}, {565, {0.6784f, 0.9786f, 0.0027f}},
    {570, {0.7621f, 0.9520f, 0.0021f}}, {575, {0.8425f, 0.9154f, 0.0018f}},
    {580, {0.9163f, 0.8700f, 0.0017f}}, {585, {0.9786f, 0.8163f, 0.0014f}},
    {590, {1.0263f, 0.7570f, 0.0011f}}, {595, {1.0567f, 0.6949f, 0.0010f}},
    {600, {1.0622f, 0.6310f, 0.0008f}}, {605, {1.0456f, 0.5668f, 0.0006f}},
    {610, {1.0026f, 0.5030f, 0.0003f}}, {615, {0.9384f, 0.4412f, 0.0002f}},
    {620, {0.8544f, 0.3810f, 0.0002f}}, {625, {0.7514f, 0.3210f, 0.0001f}},
    {630, {0.6424f, 0.2650f, 0.0000f}}, {635, {0.5419f, 0.2170f, 0.0000f}},
    {640, {0.4479f, 0.1750f, 0.0000f}}, {645, {0.3608f, 0.1382f, 0.0000f}},
    {650, {0.2835f, 0.1070f, 0.0000f}}, {655, {0.2187f, 0.0816f, 0.0000f}},
    {660, {0.1649f, 0.0610f, 0.0000f}}, {665, {0.1212f, 0.0446f, 0.0000f}},
    {670, {0.0874f, 0.0320f, 0.0000f}}, {675, {0.0636f, 0.0232f, 0.0000f}},
    {680, {0.0468f, 0.0170f, 0.0000f}}, {685, {0.0329f, 0.0119f, 0.0000f}},
    {690, {0.0227f, 0.0082f, 0.0000f}}, {695, {0.0158f, 0.0057f, 0.0000f}},
    {700, {0.0114f, 0.0041f, 0.0000f}}, {705, {0.0081f, 0.0029f, 0.0000f}},
    {710, {0.0058f, 0.0021f, 0.0000f}}, {715, {0.0041f, 0.0015f, 0.0000f}},
    {720, {0.0029f, 0.0010f, 0.0000f}}, {725, {0.0020f, 0.0007f, 0.0000f}},
    {730, {0.0014f, 0.0005f, 0.0000f}}, {735, {0.0010f, 0.0003f, 0.0000f}},
    {740, {0.0007f, 0.0002f, 0.0000f}}, {745, {0.0005f, 0.0002f, 0.0000f}},
    {750, {0.0003f, 0.0001f, 0.0000f}}, {755, {0.0002f, 0.0001f, 0.0000f}},
    {760, {0.0001f, 0.0001f, 0.0000f}}, {765, {0.0001f, 0.0001f, 0.0000f}},
    {770, {0.0001f, 0.0000f, 0.0000f}}, {775, {0.0000f, 0.0000f, 0.0000f}},
    {780, {0.0000f, 0.0000f, 0.0000f}}
};

class Color
{
  public:
    enum Type { RGB, SPECTRUM };
    Type getType() const { return type; }

  private:
    Type type;    

    // RGB data
    real r;   // Red
    real g;   // Green
    real b;   // Blue

    // Spectrum data: wavelength -> intensity
    std::map<float, float> spectrum;

    // Conversion Method from Spectrum to XYZ
    // This function integrates the weighted spectrum data 
    // against the color matching functions.
    static std::tuple<float, float, float> spectrumToXYZ(
        const std::map<float, float>& spectrum
    );
 
    // Convert XYZ to RGB
    // We use the sRGB color space conversion matrix for this step.
    static std::tuple<float, float, float> xyzToRGB(float X, float Y, float Z);
  
  public:

    Color() : type(RGB), r(1.0), g(1.0), b(1.0) {}

    // RGB Color Constructor
    Color(real r, real g, real b)
        : type(RGB), r(r), g(g), b(b) {}

    // Spectrum Constructor
    Color(const std::map<float, float>& spectrumData)
        : type(SPECTRUM), spectrum(spectrumData) 
    {
        auto [R, G, B] = toRGB();
        r = R;
        g = G;
        b = B;
    }

    real getR(void);
    real getG(void);
    real getB(void);

    // Intensity
    inline real magnitude() const {
        return sqrt(r * r + g * g + b * b);
    }

    // Perceived brightness
    inline real luminance() const {
        // Human eye sensitivity weights
        return 0.2126 * r + 0.7152 * g + 0.0722 * b;  
    }

    Color& operator += (const Color &c) 
    {
        r += c.r;
        g += c.g;
        b += c.b;
        return(*this);
    }
    Color& operator *= (real scalar) 
    {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        return(*this);
    }
    const Color operator/(real scalar) const
    {
        return {
            r / scalar,
            g / scalar,
            b / scalar
        };
    }
    Color& operator /= (real scalar) 
    {
        r /= scalar;
        g /= scalar;
        b /= scalar;
        return(*this);
    }
    friend Color operator + (const Color &c1, const Color &c2)
    {
        return(Color(c1.r + c2.r,
                     c1.g + c2.g,
                     c1.b + c2.b));
    }
    friend Color operator * (const Color &c1, const Color &c2)
    {
        return(Color(c1.r * c2.r,
                     c1.g * c2.g,
                     c1.b * c2.b));
    }
    friend Color operator * (const Color &c, real scalar)
    {
        return(Color(c.r * scalar,
                     c.g * scalar,
                     c.b * scalar));
    }
    friend std::ostream& operator<<(std::ostream& os, const Color& c)
    {
        os << '(' << c.r << ", " << c.g << ", " << c.b << ')';
        return os;
    }

    std::tuple<float, float, float> toRGB() const;    
};  


#endif

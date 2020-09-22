#ifndef _COLOR_H
#define _COLOR_H

#include "util.h"

#include <iostream>

class Color
{
    real r;   // Red
    real g;   // Green
    real b;   // Blue
  
  public:
 
    Color();
    Color(real r, real g, real b);
    real getR(void);
    real getG(void);
    real getB(void);

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
};  


#endif

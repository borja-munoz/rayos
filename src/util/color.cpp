#include "color.h"

real Color::getR(void)
{
    return(this->r);
}

real Color::getG(void)
{
    return(this->g);
}

real Color::getB(void)
{
    return(this->b);
}

// Conversion Method from Spectrum to XYZ
// This function integrates the weighted spectrum data 
// against the color matching functions.
std::tuple<float, float, float> Color::spectrumToXYZ(
	const std::map<float, float>& spectrum
) 
{
	float X = 0.0f, Y = 0.0f, Z = 0.0f;

	for (const auto& [wavelength, intensity] : spectrum) {
		if (CIE_XYZ.find((int)wavelength) != CIE_XYZ.end()) {
			auto [xBar, yBar, zBar] = CIE_XYZ.at((int)wavelength);
			X += intensity * xBar;
			Y += intensity * yBar;
			Z += intensity * zBar;
		}
	}

	return {X, Y, Z};
}

// Convert XYZ to RGB
// We use the sRGB color space conversion matrix for this step.
std::tuple<float, float, float> Color::xyzToRGB(float X, float Y, float Z) 
{
	// sRGB conversion matrix from XYZ to RGB
	float r = 3.2406f * X - 1.5372f * Y - 0.4986f * Z;
	float g = -0.9689f * X + 1.8758f * Y + 0.0415f * Z;
	float b = 0.0557f * X - 0.2040f * Y + 1.0570f * Z;

	// Clamp values to [0, 1]
	r = std::max(0.0f, std::min(1.0f, r));
	g = std::max(0.0f, std::min(1.0f, g));
	b = std::max(0.0f, std::min(1.0f, b));

	return std::make_tuple(r, g, b);
}

std::tuple<float, float, float> Color::toRGB() const 
{
	// How to use it:
	// Color spectralColor({
	//     {400, 0.1f}, {500, 0.3f}, {600, 0.5f}, {700, 0.2f}
	// });
	// auto [r, g, b] = spectralColor.toRGB();

	if (type == RGB) {
		return std::make_tuple(r, g, b);
	} else if (type == SPECTRUM) {
		auto [X, Y, Z] = spectrumToXYZ(spectrum);
		return xyzToRGB(X, Y, Z);
	}
	return {0.0f, 0.0f, 0.0f};
}    

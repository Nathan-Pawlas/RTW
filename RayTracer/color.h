#ifndef COLOR_H
#define COLOR_H

#include "utils.h"
#include <iostream>

using color = vec3;

inline double gamma_correction(double linear_component)
{
	return sqrt(linear_component);
}

//Takes sum total of light for the pixel and number of samples and returns a final color value
void write_color(std::ostream& out, color pixel_color, int num_samples) 
{
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	//Scale the colors by num of samples
	auto scale = 1.0 / num_samples;
	r *= scale; 
	g *= scale; 
	b *= scale;

	//Apply Gamma Correction
	r = gamma_correction(r); 
	g = gamma_correction(g); 
	b = gamma_correction(b);

	//TO DO: Adjust This To Using a Windowed System instead of
	// writing out to a file
	//Write out the color component to "image.ppm"
	static const interval intensity(0.000, 0.999);
	out << static_cast<int>(256 * intensity.clamp(r)) << ' '
		<< static_cast<int>(256 * intensity.clamp(g)) << ' '
		<< static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif
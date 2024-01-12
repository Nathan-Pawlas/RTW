#include "Renderer.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include "ray.h"
#include "Math/vec3.h"
#include "color.h"
#include "Primatives/Sphere.h"

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

Renderer::Renderer()
{

}

bool Renderer::Render(Image& outputImage)
{
    //Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = outputImage.GetXSize();

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    //Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    auto cam_center = point3(0, 0, 0);

    //Vectors Across Horiz and Down the Vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = cam_center
        - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    auto xSize = outputImage.GetXSize();
    auto ySize = outputImage.GetYSize();

    //Iterate through all pixels
    for (int y = 0; y < ySize; ++y) {
        std::cout << std::fixed << std::setprecision(3) << ((float)y/ySize) * 100 << '%' << std::endl;
        for (int x = 0; x < xSize; ++x) {
            //Cast a ray
            auto pixel_center = pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);
            auto ray_direction = pixel_center - cam_center;
            ray r(cam_center, ray_direction);

            //Set Pixel Color
            color pixel_color = ray_color(r);
            outputImage.SetPixel(x, y, pixel_color.x(), pixel_color.y(), pixel_color.z());
        }
    }

    std::cout << "\rDone.                 \n" << std::flush;
	
	return true;
}
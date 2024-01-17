#pragma once

#include "utils.h"
#include "color.h"
#include "hittable.h"
#include <iostream>
#include <iomanip>
#include "stb_image_write.h"
#define CHANNEL_NUM 3


class camera
{
public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width = 100;  // Rendered image width in pixel count
    int num_samples = 10; //Count of samples for each pixel
    int max_depth = 10; //Max Ray Bounces

    void render(const hittable& world) {
        initialize();
       

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << ((float)j / image_height) * 100 << "%" << std::endl;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int n = 0; n < num_samples; n++)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, num_samples);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int    image_height;   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

	color ray_color(const ray& r, int depth, const hittable& world) const
	{
		hit_record rec;

        if (depth <= 0)
            return color(0, 0, 0);

		if (world.hit(r, interval(0.001, infinity), rec))
		{
            vec3 dir = rec.normal + random_unit_vector();
            return 0.7 * ray_color(ray(rec.p, dir), depth - 1, world);
		}

		vec3 unit_dir = unit_vector(r.direction());
		auto a = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - a) * color(1, 1, 1) + a * color(0.6, 0.4, 0.8);
	}

    ray get_ray(int i, int j) const
    {
        //Get a Random Point from a Square Area Around Pixel
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();
        
        //Cast Ray from Cam's center to Sample Point
        auto ray_dir = pixel_sample - center;
        return ray(center, ray_dir);
    }

    vec3 pixel_sample_square() const
    {
        //Return a Random Sample Point based around Pixel
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};
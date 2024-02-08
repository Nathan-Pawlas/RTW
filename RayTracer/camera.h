#pragma once

#include "utils.h"
#include "hittable.h"
#include "material.h"
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
    color background; 
    double vfov = 90; //Vertical FOV
    point3 camPos = point3(0, 0, -1);
    point3 lookat = point3(0, 0, 0); //Point Cam is Looking At
    vec3 up = vec3(0, 1, 0); //Relative 'UP' Direction

    double defocus_angle = 0;
    double focus_dist = 10;

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
    vec3   u, v, w;        // Cam Frame Basis Vectors
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = camPos;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        //Calculate u,v,w unit basis vectors for Cam coordinate frame
        w = unit_vector(camPos - lookat);
        u = unit_vector(cross(up, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        //Calc Cam Defocus Disk Basis Vectors
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle) / 2);
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

	color ray_color(const ray& r, int depth, const hittable& world) const
	{
		hit_record rec;

        //Limit how many ray bounces we have
        if (depth <= 0)
            return color(0, 0, 0);

        //If the ray doesn't hit anything, return the background color
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered; 
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

        return color_from_emission + color_from_scatter;
	}

    ray get_ray(int i, int j) const
    {
        //Get a Random Point from a Square Area Around Pixel
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();
        
        //Cast Ray from Cam's center to Sample Point
        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_dir = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_dir, ray_time);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    vec3 pixel_sample_square() const
    {
        //Return a Random Sample Point based around Pixel
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};
// pathtracer.cpp : Defines the entry point for the application.
#include "common_utility.h"

#include "pathtracer.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>

using namespace std;

//adding color and vector utility functions

#include "Hittable.h"
#include "hittable_list.h"
#include "sphere.h"
// #include "color.h"
// #include "ray.h"
// #include "vec3.h"
//#include "Windows.h"

//function to add a sphere to the scene if the ray hits within the sphere radius
// double hit_sphere(const point3& center, double radius, const ray& r) {
//     vec3 oc = center - r.origin(); //vector from ray origin to sphere center
//     auto a = r.direction().length_squared(); //length squared of the ray direction vector
//     auto b = dot(r.direction(), oc);
//     auto c = oc.length_squared() - (radius * radius);
//     auto discriminant = (b * b) - (a * c); //discriminant of the quadratic equation
//     //std::cout << "Discriminant: " << discriminant << std::endl;
//     //return (discriminant >= 0); //if discriminant is positive, the ray hits the sphere

//     if (discriminant < 0) {
//         return -1.0; //no hit
//     }
//     else {
//         return (b - std::sqrt(discriminant)) / a; //return the distance to the hit point
//     }
// }

color ray_color(const ray& r, const hittable& world) {
    //if the ray hits the sphere, return a color based on the hit
     //if (hit_sphere(point3(0, -1, -1), 1, r)) {
     //    return color(1, 0, 0); //return a sphere of red color 
     //}
    // auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    // if (t > 0.0) {
    //     //calculate the normal at the hit point
    //     vec3 normal = unit_vector(r.at(t) - vec3(0, 0, -1));

    //     //return a color based on the normal
    // //   return 0.5 * color(1, 0, 0); //normal is in range [-1, 1], so we add 1 to shift it to [0, 2] and then scale by 0.5
    // vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
    // return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    //}

    hit_record rec;
    //checks for the world as a shared pointed instead of going through all objects one at a time
    if (world.hit(r, 0, infinity, rec)) {
        // Map normal from [-1, 1] to [0, 1] color range and clamp
        color result = 0.5 * (rec.normal + color(1,1,1));
        return color(
            std::fmax(0.0, std::fmin(1.0, result.x())),
            std::fmax(0.0, std::fmin(1.0, result.y())),
            std::fmax(0.0, std::fmin(1.0, result.z()))
        );
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
    //MARK: FILE PATH
    std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;
    // build an absolute, resolved path for the output file so failures are clear
    std::filesystem::path outPath = std::filesystem::absolute(std::filesystem::current_path()  / "imagetest_2.ppm");
    std::cout << "Resolved output path: " << outPath << std::endl;
    std::ofstream image(outPath);

    if (!image.is_open()) {
        std::cerr << "Error: Image file not created" << std::endl;
        return 1;
    }
    //SetConsoleOutputCP(CP_UTF8);

      // Image -- rendered image set- up
      //iniatilising aspect ratio
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    //calculate image height and ensure that it's at least 1
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    //MARK: WORLD
    hittable_list world;

    //sphere obejct 1
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));

    //sphere object 2 - that is the world basically
    world.add(make_shared<sphere>(point3(0,-100.5, -1), 100));

    //camera 
    auto focal_length = 1.0;

    //viewport widths less than 1 is ok since they are real valued
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (aspect_ratio);
    auto camera_center = point3(0, 0, 0); //initialising camera location

    //calculating the vectors across the horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    //change in positions
    //calculating the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    //calculating position of upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
    auto pixel00_loc = viewport_upper_left + (0.5 * (pixel_delta_u + pixel_delta_v));

    // Render
    /*std::cout << "P3\n" << image_width <<" "<< image_height << "\n255\n";*/
    image << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << image_height - j << " " << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;

            ray r(camera_center, ray_direction);

            //sets the colors by passing the ray through the list of hittable objs - world
            color pixel_color = ray_color(r, world);

            write_color(image, pixel_color);
            // if (i == image_width / 2 && j == image_height / 2) {
            // pixel_color = color(1, 0, 0);  // Force center pixel red

            // write_color(image, pixel_color);
            // std::clog << "\nCenter pixel set to red.\n";
            // }
        }
    }
    image.close();
    std::clog << "\nDone.      \n";

    return 0;
}

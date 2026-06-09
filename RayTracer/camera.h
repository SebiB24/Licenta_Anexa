//
// Created by sebyc on 12/03/2026.
//

#ifndef IMAGE_FILE_CAMERA_H
#define IMAGE_FILE_CAMERA_H

#include <fstream>
#include <vector>
#include "hittable.h"
#include "material.h"
#include "quad.h"

class camera {
public:
/// PUBLIC ATRIBUTES
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    color background;

    double vfov = 90;
    point3 lookfrom = point3(0,0,0);
    point3 lookat   = point3(0,0,-1);
    vec3   vup      = vec3(0,1,0);

    double defocus_angle = 0; // Variation angle of rays through each pixel
    double focus_dist = 10; // Distance from camera lookfrom point to plane of perfect focus

/// RENDER
    void render(const hittable& world){
        ofstream out_file("image.ppm");
        initialize();
        out_file << "P3\n"<< image_width << " " << image_height << "\n255\n";

        for(int i = 0 ; i < image_height; i++){
            cout << "\rScanlines remaining: "<<(image_height - i)<< ' ' << flush; // log to keep trak of progress
            for(int j = 0; j < image_width; j ++)
            {
//                handdle_pixel_random_rays(i, j, world, out_file, light_source);
//                handdle_pixel_Gauss(i, j, world, out_file, light_source, 9);
//                handdle_pixel_Adaptive(i,j,world,out_file, light_source, 2); // implementare ineficienta =(

            }
        }

        cout<<"\rDone.                        \n";
    }

    void render(const hittable& world, const quad& light_source){
        ofstream out_file("image.ppm");
        initialize();
        out_file << "P3\n"<< image_width << " " << image_height << "\n255\n";

        for(int i = 0 ; i < image_height; i++){
            cout << "\rScanlines remaining: "<<(image_height - i)<< ' ' << flush; // log to keep trak of progress
            for(int j = 0; j < image_width; j ++)
            {
//                handdle_pixel_random_rays(i, j, world, out_file, light_source)
                handdle_pixel_Gauss(i, j, world, out_file, light_source, 3);
//                handdle_pixel_Adaptive(i,j,world,out_file, light_source, 2); // implementare ineficienta =(

            }
        }

        cout<<"\rDone.                        \n";
    }
private:
/// PRIVATE ATRIBUTES
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3   pixel_delta_u;
    vec3   pixel_delta_v;
    double pixel_sample_scale;
    vec3 u, v, w;
    vec3 defocus_disk_u; // Defocus disk horizontal radius
    vec3 defocus_disk_v; // Defocus disk vertical radius

/// INITIALIZE
    void initialize(){
        // setting image_height
        image_height = int(image_width / aspect_ratio );
        image_height = (image_height < 1) ? 1: image_height;

        pixel_sample_scale = 1.0/samples_per_pixel;

        center = lookfrom;

        // determine viewport dimensions
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * ( double(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Viewport vectors
        // auto viewport_u = vec3(viewport_width, 0, 0);
        // auto viewport_v = vec3(0, -viewport_height, 0);
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // Viewpoert deltas
        pixel_delta_u = viewport_u/image_width; // divide by image size !!
        pixel_delta_v = viewport_v/image_height;

        // Viewport upper left pixel
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * ( pixel_delta_u + pixel_delta_v );

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    void handdle_pixel_random_rays(int i, int j, const hittable& world, ofstream& out_file, const quad& light_source){
        color pixel_color = color(0, 0, 0);
        for(int sample = 1; sample <= samples_per_pixel; sample++){
            ray r = get_ray_with_exact_offset(i, j, sample_square_random());
            pixel_color += ray_color(r,max_depth, world);
//            pixel_color += ray_color(r,max_depth, world, light_source);
        }
        write_color(out_file, pixel_sample_scale * pixel_color);
    }

    void handdle_pixel_Gauss(int i, int j, const hittable& world, ofstream& out_file, const quad& light_source, int degree) {
        std::vector<double> nodes;
        std::vector<double> weights;

        switch (degree) {
            case 9:
                nodes = {-0.4840801, -0.4180156, -0.3066857, -0.1621267, 0.0,
                         0.1621267,  0.3066857,  0.4180156,  0.4840801};

                weights = {0.0406372, 0.0903241, 0.1303053, 0.1561735, 0.1651197,
                           0.1561735, 0.1303053, 0.0903241, 0.0406372};
                break;

            default:
                degree = 3;
                nodes = {-0.387298, 0.0, 0.387298};
                weights = {0.277777, 0.444444, 0.277777};
                break;
        }

        color pixel_color(0, 0, 0);

        for (int xi = 0; xi < degree; xi++) {
            for (int yi = 0; yi < degree; yi++) {
                vec3 offset(nodes[xi], nodes[yi], 0);
                double weight = weights[xi] * weights[yi];
                ray r = get_ray_with_exact_offset(i, j, offset);
                pixel_color += weight * ray_color(r, max_depth, world);
//                pixel_color += weight * ray_color(r, max_depth, world, light_source); //f(x) = ray_color + get_ray
            }
        }
        write_color(out_file, pixel_color);
    }

    color adaptive_helper(int i, int j, double u0, double u1, double v0, double v1, const hittable& world, const quad& light_source, int depth, int max_adaptive_depth){
        double um = (u0 + u1) /2.0;
        double vm = (v0 + v1) /2.0;
        vec3 pts[9] = {
                vec3(u0, v0, 0), vec3(um, v0, 0), vec3(u1, v0, 0),
                vec3(u0, vm, 0), vec3(um, vm, 0), vec3(u1, vm, 0),
                vec3(u0, v1, 0), vec3(um, v1, 0), vec3(u1, v1, 0),
        };
        double weights[9] = {
                1.0/36.0,  4.0/36.0, 1.0/36.0,
                4.0/36.0, 16.0/36.0, 4.0/36.0,
                1.0/36.0,  4.0/36.0, 1.0/36.0
        };
        color c[9];
        color simpson_result(0,0,0);
        double min = 9999.0;
        double max = -9999.0;
        for(int k = 0; k < 9; k++){
            ray r = get_ray_with_exact_offset(i, j, pts[k]);
            c[k] = ray_color(r, max_depth, world);
//            c[k] = ray_color(r, max_depth, world, light_source);
            simpson_result += weights[k] * c[k];

            double intensity = c[k].x() + c[k].y() + c[k].z();
            if (intensity < min) min = intensity;
            if (intensity > max) max = intensity;
        }
        double tolerance = 0.3;
        if (depth >= max_adaptive_depth || (max - min) < tolerance) {
            return simpson_result;
        }
        color c_SW = adaptive_helper(i, j, u0, um, v0, vm, world, light_source, depth + 1, max_adaptive_depth);
        color c_SE = adaptive_helper(i, j, um, u1, v0, vm, world, light_source, depth + 1, max_adaptive_depth);
        color c_NW = adaptive_helper(i, j, u0, um, vm, v1, world, light_source, depth + 1, max_adaptive_depth);
        color c_NE = adaptive_helper(i, j, um, u1, vm, v1, world, light_source, depth + 1, max_adaptive_depth);
        return 0.25 * (c_SW + c_SE + c_NW + c_NE);

    }
    void handdle_pixel_Adaptive(int i, int j, const hittable& world, ofstream& out_file, const quad& light_source,int max_adaptive_depth){
        color pixel_color = adaptive_helper(i, j, -0.5, 0.5, -0.5, 0.5, world, light_source, 0, max_adaptive_depth);
        write_color(out_file, pixel_color);
    }

    ray get_ray_with_exact_offset(int i, int j, const vec3& offset) const{
        auto pixel_sample = pixel00_loc + ((j + offset.x()) * pixel_delta_u) + ((i + offset.y()) * pixel_delta_v);
        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square_random() const{
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }


    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

/// RAY COLOR

//    color ray_color(const ray& r, int depth, const hittable& world) {
//        hit_record rec;
//        // Daca lovim un obiect
//        if (world.hit(r, interval(0.001, infinity), rec)) {
//            // Returnam culoarea direct pe baza normalei (FARA ALEATOR, FARA BOUNCES)
//            return 0.5 * color(rec.N.x() + 1, rec.N.y() + 1, rec.N.z() + 1);
//        }
//
//        // Daca nu lovim nimic, returnam fundalul (ex: un gradient simplu albastru-alb)
//        vec3 unit_direction = unit_vector(r.direction());
//        auto t = 0.5 * (unit_direction.y() + 1.0);
//        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//    }


    color ray_color(const ray& r, int depth,  const hittable& world) const {
        if(depth <= 0)
            return color(0,0,0);
        hit_record hr;
    //        0.001 so that with misscalculated origins that end up inside the object they hit and hit it again at a verry low time (t)
        if (!world.hit(r, interval(0.001, infinity), hr))
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = hr.mat->emitted(hr.u, hr.v, hr.p);

        if (!hr.mat->scatter(r, hr, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);
        return color_from_emission + color_from_scatter;
    }


//    color ray_color(const ray& r, int depth, const hittable& world, const quad& light_source, bool is_bounced = false) const {
//        if(depth <= 0)
//            return color(0,0,0);
//
//        hit_record hr;
//        if (!world.hit(r, interval(0.001, infinity), hr))
//            return color(0,0,0); // background negru
//
//        color color_from_emission = hr.mat->emitted(hr.u, hr.v, hr.p);
//
//        ray scattered;
//        color attenuation;
//
//        // daca am lovit o sursa de lumina
//        if (!hr.mat->scatter(r, hr, attenuation, scattered)) {
//            // prevenirea numararii duble
//            if (is_bounced) {
//                return color(0, 0, 0);
//            }
//            return color_from_emission;
//        }
//
//        // 1. COMPONENTA DIRECTA (Simpson 2D)
//        color raw_direct_light = compute_direct_light_simpson(hr, light_source, world);
//
//        // 2. COMPONENTA INDIRECTA
//        color indirect_light = ray_color(scattered, depth - 1, world, light_source, true);
//
//        // HIBRIDIZARE:
//        return color_from_emission + (attenuation * (raw_direct_light + indirect_light));
//    }

// Calculeaza lumina directa folosind Simpson 2D
    color compute_direct_light_simpson(const hit_record& hr, const quad& light, const hittable& world) const {
        color total_light(0, 0, 0);

        // Coordonatele parametrice (s, t)
        double nodes[3] = {0.0, 0.5, 1.0};

        // Ponderile
        double weights[3] = {1.0 / 6.0, 4.0 / 6.0, 1.0 / 6.0};

        // intensitatea materialului becului
        color light_emission = light.mat->emitted(0, 0, point3(0,0,0));

        // Parcurgem grila 3x3 de pe sursa de lumina
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                vec3 light_point = light.Q + (nodes[i] * light.u) + (nodes[j] * light.v);

                // 2. Calculam directia razei de umbra si distanta
                vec3 dir_to_light = light_point - hr.p;
                double distance_squared = dir_to_light.length_squared();

                // previne divizarea la zero si singularitatile
                const double min_distance_squared = 100.0;
                if (distance_squared < min_distance_squared) {
                    distance_squared = min_distance_squared;
                }
                double distance = sqrt(distance_squared);
                dir_to_light = unit_vector(dir_to_light);

                double light_cos_theta = dot(light.normal, -dir_to_light);
                // Daca unghiul e negativ, spatele becului sau marginea acestuia incearca sa ilumineze
                if (light_cos_theta <= 0.0) {
                    continue;
                }

                // 3. Lansam raza de umbra (offset pt eliminarea erorilor floating point)
                ray shadow_ray(hr.p + hr.N * 0.001, dir_to_light);
                hit_record shadow_rec;

                // Daca raza nu loveste nimic pe drum
                if (!world.hit(shadow_ray, interval(0.001, distance - 0.001), shadow_rec)) {

                    // cosinusul unghiului de incidenta pe suprafata
                    double cos_theta = fmax(dot(hr.N, dir_to_light), 0.0);

                    // Pondere Simpson 2D pentru acest punct
                    double weight_2D = weights[i] * weights[j];

                    // Calculam contributia de lumina
                    total_light += weight_2D * light_emission * cos_theta * light_cos_theta * (light.area / distance_squared);
                }
            }
        }

        return total_light;
    }

};

#endif //IMAGE_FILE_CAMERA_H

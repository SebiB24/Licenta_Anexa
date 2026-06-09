//
// Created by sebyc on 16/03/2026.
//

#ifndef IMAGE_FILE_MATERIAL_H
#define IMAGE_FILE_MATERIAL_H


class material {
public:
    virtual ~material() = default;

    virtual  color emitted(double u, double v, const point3& p) const{
        return color(0,0,0);
    }

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};
class lambertian : public material {
public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& hr, color& attenuation, ray& scattered)
    const override {
        auto scatter_direction = hr.N + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = hr.N;

        scattered = ray(hr.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& albedo, const double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1){}

    bool scatter(const ray& r_in, const hit_record& hr, color& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(r_in.direction(), hr.N);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(hr.p, reflected);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material{
public:
    dielectric(double refraction_index) : refraction_index(refraction_index){}

    bool scatter(const ray& r_in, const hit_record& hr, color& attenuation, ray& scattered)
    const override{
        attenuation = color(1.0, 1.0, 1.0);
        double ri = hr.front_face ? (1.0/refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, hr.N), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;
        // we only refract when possible
        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, hr.N);
        else
            direction = refract(unit_direction, hr.N, ri);

        scattered = ray(hr.p, direction);
        vec3 refracted = refract(unit_direction, hr.N, ri);

        scattered = ray(hr.p, refracted);
        return true;
    }
private:
    double refraction_index;

    static double reflectance(double cosine, double refraction_index){
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
};

class diffuse_light : public material{
public:
    diffuse_light(color c) : emit(c){}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        return false;
    }

    color emitted(double u, double v, const point3& p) const override {
        return emit;
    }

private:
    color emit;
};



#endif //IMAGE_FILE_MATERIAL_H

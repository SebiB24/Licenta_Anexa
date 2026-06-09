//
// Created by sebyc on 03/03/2026.
//

#ifndef IMAGE_FILE_SPHERE_H
#define IMAGE_FILE_SPHERE_H

#include "hittable.h"

class sphere: public hittable{
public:
    sphere(const point3& center, double radius, shared_ptr<material> mat)
            : center(center), radius(std::fmax(0,radius)), mat(mat) {}

    bool hit( const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
//    auto a = dot(r.direction(), r.direction());
        auto a = r.direction().length_squared();
//    auto b = -2.0 * dot(r.direction(), oc);
//    variable change: b = -2h
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;
        auto discriminant = h*h - a*c;
        if(discriminant < 0)
            return false;

        auto sqrtd = sqrt(discriminant);

        /// Find the nearest root that lies in the acceptable range
        auto root = (h - sqrtd)/a;
        if(!ray_t.contains(root)){
            root = (h + sqrtd)/a;
            if(!ray_t.contains(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
//        // the normal will always face outwards like this
//        rec.N = (rec.p - center)/radius;
        vec3 outward_normal = (rec.p - center)/radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};

#endif //IMAGE_FILE_SPHERE_H

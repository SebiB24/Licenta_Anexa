//
// Created by sebyc on 03/03/2026.
//

#ifndef IMAGE_FILE_HITTABLE_H
#define IMAGE_FILE_HITTABLE_H
class material;

class hit_record{
public:
    point3 p;
    vec3 N;
    shared_ptr<material> mat;
    double t;
    bool front_face;

    double u;
    double v;

    // this functions makes it so that the normal will start facing inward if the ray comes from within the object
    void set_face_normal(const ray& r, const vec3& outward_normal){
        front_face = dot(r.direction(), outward_normal) < 0;
        N = front_face ? outward_normal : -outward_normal;
    }
};

class hittable{
public:
    // virtual makes the compiler look for the method implementation in the child class
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif //IMAGE_FILE_HITTABLE_H

//
// Created by sebyc on 26/02/2026.
//

#ifndef IMAGE_FILE_RAY_H
#define IMAGE_FILE_RAY_H


class ray{
public:
    ray(){}
    ray(const point3& origin, const vec3& direction): orig(origin), dir(direction){}

    const point3& origin() const {return orig;}
    const vec3& direction() const {return dir;}

    point3 at(double t) const {
        return orig + t*dir;
    }
private:
    point3 orig;
    vec3 dir;
};

#endif //IMAGE_FILE_RAY_H

//
// Created by sebyc on 04/03/2026.
//

#ifndef IMAGE_FILE_HITTABLE_LIST_H
#define IMAGE_FILE_HITTABLE_LIST_H

#include "hittable.h"

#include <vector>


class hittable_list : public hittable{
public:
    // by wrapping the hittable in a shared_ptr we no longer have to worry about deleting instances
    // shared_ptr keeps count of how many active pointers to that memory section there are and when
    // it hits 0 the object is auto deleted
    std::vector<shared_ptr<hittable>> objects;

    hittable_list(){}
    hittable_list(shared_ptr<hittable> object){ add(object);}
    void clear(){objects.clear();}

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
        hit_record temp_record;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for(const auto& object: objects){
            // change ray_tmax to closest_so_far to find the closest point to the camera
            if(object->hit(r, interval(ray_t.min, closest_so_far), temp_record)){
                hit_anything = true;
                closest_so_far = temp_record.t;
                rec = temp_record;
            }
        }
        return hit_anything;
    }

};

#endif //IMAGE_FILE_HITTABLE_LIST_H

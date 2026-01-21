#ifndef HITTABLE_LIST
#define HITTABLE_LIST

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
    public:
        //vector  of (shared)pointers to hittable objects (eg: sphere)
        std::vector<shared_ptr<hittable>> objects;

        //constructor
        hittable_list() {}

        //overloaded constructor
        hittable_list(shared_ptr<hittable> object) {add(object);}

        //public methods for the list of objects
        void clear() {objects.clear();}
        
        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        //override hit method for hittable
        bool hit (const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_tmax;

            for (const auto& object : objects) {
                if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }
};

#endif
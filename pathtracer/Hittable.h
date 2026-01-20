#ifndef HITTABLE
#define HITTABLE

//HITTABLE - Abstract class that allows an object to ne hit
#include "ray.h"

//to keep count of the objects created
class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
};

class hittable {
    public:
        virtual ~hittable() = default;
        
        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif
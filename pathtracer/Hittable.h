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
        bool front_face;

        void set_face_normal(const ray& r , const vec3& outward_normal) {
            //sets the hit record normal vector
            //NOTE: the parameter `outward_normal` is assumed to have length
            
            //front_face = dot(r.direction(), outward_normal) < 0;
            //normal = front_face ? outward_normal : -outward_normal;
            //SURFACE-NORMAL GEOMETRY - front face tracking
            if (dot(r.direction(), outward_normal) > 0) {
                //ray is inside the sphere
                normal = -outward_normal;
                front_face = false;
            } else {
                //ray is outside the sphere
                normal = outward_normal;
                front_face = true;
            }
        }
};

class hittable {
    public:
        virtual ~hittable() = default;
        
        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif
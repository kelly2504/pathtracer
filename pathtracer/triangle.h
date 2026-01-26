#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "common_utility.h"
#include "Hittable.h"

class Triangle : public hittable{
    public:
        vec3 normal;
        Triangle(const point3& p1, const point3& p2, const point3& p3) : v1(p1), v2(p2), v3(p3) {
            normal = cross((v2 - v1), (v3 - v1));
        }

        //uses Moller-Trumvore algorithm
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            //rays will hit on a triangle when lights hits within an edge 
            //edge - v1 -> v2 - v2 -> v3 , v3 -> v1 across an x, y, z field
            constexpr double epsilon = 1e-8;

            vec3 edge1 = v2 - v1;
            vec3 edge2 = v3 - v1;

            //find cross product -- in vec3.h
            vec3 h = cross(r.direction(), edge2);

            //find dot  -- in vec3.h
            double a = dot (edge1, h);

            if (std::fabs(a) < epsilon) {
                //ray parallel to triangle
                return false;
            }

            //magnitude
            double f = 1.0 / a;

            vec3 s = r.origin() - v1;

            //find the unit vector
            double u = f * dot(s,h);

            if (u < 0.0 || u > 1.0 ) {
                return false;
            }

            vec3 q = cross(s, edge1);
            double v = f * dot(r.direction(), q);

            if (v < 0.0 || u + v > 1.0) {
                return false;
            }

            double t = f * dot(edge2, q);

            if (t < ray_t.min || ray_t.max) {
                return false;
            }

            //set hit_record
            rec.t = t;
            rec.p = r.at(t);
            rec.normal = normal;

            if (dot(rec.normal, r.direction()) > 0) {
                rec.normal = rec.normal * -1.0;
            }

            return true;
        }
    
    private:
        point3 v1, v2, v3;
};

//MAKE AN INTERSECTION FUNCTION

//cross function -- already exists in vec3 
// vec3 cross(const vec3 v1, const vec3 v2) {
//     return {
//         v1.y() * v2.z() - v2.y() * v1.z(),
//         v1.x() * v2.z() - v2.x() * v1.z(),
//         v1.y() * v2.x() - v2.y() * v1.x()
//     };
    
// }
#endif
#pragma once

#include "object.h"
#include "materials/isotropic.h"

#include <limits>

class constant_medium : public object
{
    public:
        constant_medium(object* pObj, float d, texture* pTex);
        bool hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const override;
        bool bounding_box(const float t0, const float t1, aabb& box) const override;

    private:
        object* pBoundary;
        float density;
        material* pPhase_function;
};

 constant_medium::constant_medium(object* pObj, float d, texture* pTex): pBoundary(pObj), density(d)
 {
     pPhase_function = new isotropic(pTex);
 }

bool constant_medium::hit(const ray& r, const float t_min, const float t_max, hit_record& rec) const
{
    bool db = (math::rand() < 0.00001);
    db = false;
    hit_record rec1, rec2;
    if (pBoundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) 
    { 
        if (pBoundary->hit(r, rec1.t+0.0001, FLT_MAX, rec2)) 
        {
            if (db) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << "\n";
            if (rec1.t < t_min)
                rec1.t = t_min;
            if (rec2.t > t_max)
                rec2.t = t_max;
            if (rec1.t >= rec2.t)
                return false;
            if (rec1.t < 0)
                rec1.t = 0;
            float distance_inside_boundary = (rec2.t - rec1.t)*r.direction().length();
            float hit_distance = -(1/density)*log(math::rand()); 
            if (hit_distance < distance_inside_boundary) 
            {
                if (db) std::cerr << "hit_distance = " <<  hit_distance << "\n";
                    rec.t = rec1.t + hit_distance / r.direction().length(); 
                if (db) std::cerr << "rec.t = " <<  rec.t << "\n";
                    rec.p = r.point_at_parameter(rec.t);
                //if (db) std::cerr << "rec.p = " <<  rec.p << "\n";
                    rec.normal = glm::vec3(1,0,0);  // arbitrary
                    rec.pMaterial = pPhase_function;
                    return true;
            }
        }
    }
return false;
}

bool constant_medium::bounding_box(const float t0, const float t1, aabb& box) const
{
    return pBoundary->bounding_box(t0, t1, box);
}
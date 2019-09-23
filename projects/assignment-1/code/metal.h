//
// Created by benves-7 on 9/11/19.
//

#ifndef METAL_H
#define METAL_H


#include "material.h"

class metal : public material {
public:
    metal(const Vector4D& a, float f = 0) : albedo(a) {if (f < 1) fuzz = f; else fuzz = 1;}
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vector4D& attenuation, ray& scattered) const {
        Vector4D reflected = reflect(r_in.direction().Normalize(), rec.normal);
        scattered = ray(rec.p, reflected + random_in_unit_sphere()*fuzz);
        attenuation = albedo;
        return scattered.direction().Dot(rec.normal) > 0;
    }
    Vector4D albedo;
    float fuzz;
};


#endif //PATH_TRACING_METAL_H

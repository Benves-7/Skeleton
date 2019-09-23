//
// Created by benves-7 on 9/11/19.
//

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H


#include "material.h"
#include "sphere.h"

class lambertian : public material {
public:
    lambertian(const Vector4D a) : albedo(a) {};
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vector4D& attenuation, ray& scattered) const {
        Vector4D target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }

    Vector4D albedo;
};


#endif //PATH_TRACING_LAMBERTIAN_H

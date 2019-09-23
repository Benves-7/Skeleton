//
// Created by benves-7 on 9/11/19.
//

#ifndef MATERIAL_H
#define MATERIAL_H


#include "hitable.h"
#include "MathLib.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vector4D& attenuation, ray& scattered) const = 0;
};

Vector4D reflect(const Vector4D& v, const Vector4D& n){
    return v - n * v.Dot(n) * 2;
}

bool refract(const Vector4D& v, const Vector4D& n, float ni_over_nt, Vector4D& refracted) {
    Vector4D uv = Vector4D::unit_vector(v);
    float dt = uv.Dot(n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = (uv - n*dt)*ni_over_nt - n * sqrt(discriminant);
        return true;
    } else
        return false;
}
float schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1-cosine), 5);
}

#endif //PATH_TRACING_MATERIAL_H

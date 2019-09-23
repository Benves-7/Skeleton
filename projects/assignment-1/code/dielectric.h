//
// Created by benves-7 on 9/11/19.
//

#ifndef DIELECTRIC_H
#define DIELECTRIC_H


#include "material.h"

class dielectric : public material {
public:
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vector4D& attenuation, ray& scattered) const {
        Vector4D outward_normal;
        Vector4D reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = Vector4D(1.0,1.0,1.0);
        Vector4D refracted;
        float reflect_prob;
        float cosine;
        if (r_in.direction().Dot(rec.normal) > 0)
        {
            outward_normal = rec.normal.inverse();
            ni_over_nt = ref_idx;
            cosine = ref_idx * r_in.direction().Dot(rec.normal) / r_in.direction().Length();
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -r_in.direction().Dot(rec.normal) / r_in.direction().Length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, reflected))
        {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else
        {
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.0;
        }
        if (drand48() < reflect_prob)
        {
            scattered = ray(rec.p, reflected);
        }
        else
        {
            scattered = ray(rec.p, reflected);
        }
        return true;
    }
    float ref_idx;
};


#endif //PATH_TRACING_DIELECTRIC_H

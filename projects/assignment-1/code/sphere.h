//
// Created by benves-7 on 9/11/19.
//

#ifndef SPHERE_H
#define SPHERE_H


#include "hitable.h"
#include "material.h"

class sphere : public hitable {
public:
    sphere(){}
    sphere(Vector4D cen, float r, material* mat) : center(cen), radius(r), mat_ptr(mat) {};
    ~sphere() {
        delete mat_ptr;
    }
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    Vector4D center;
    float radius;
    material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    Vector4D oc = r.origin() - center;
    float a = r.direction().Dot(r.direction());
    float b = oc.Dot(r.direction());
    float c = oc.Dot(oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0)
    {
        float temp = (-b -sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

Vector4D random_in_unit_sphere()
{
    Vector4D p;
    do
    {
        p = Vector4D(drand48(),drand48(),drand48())* 2.0 - Vector4D(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}


#endif //PATH_TRACING_SPHERE_H

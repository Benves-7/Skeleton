//
// Created by benves-7 on 9/10/19.
//

#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"

class material;

struct hit_record
{
    float t;
    Vector4D p;
    Vector4D normal;
    material *mat_ptr;
};

class hitable{
public:
    virtual ~hitable() = default;
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};


#endif //UNTITLED2_HITABLE_H

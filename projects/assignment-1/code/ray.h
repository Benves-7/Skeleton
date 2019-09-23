//
// Created by benves-7 on 9/10/19.
//

#ifndef RAY_H
#define RAY_H

#include "MathLib.h"

class ray{
public:
    ray(){}
    ray(const Vector4D& a, const Vector4D& b) { A= a; B = b; }

    Vector4D origin() const         { return A; }
    Vector4D direction() const      { return B; }
    Vector4D point_at_parameter(float t) const { return A + B*t; }

    Vector4D A, B;
};


#endif //UNTITLED2_RAY_H

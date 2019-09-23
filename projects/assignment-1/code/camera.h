//
// Created by benves-7 on 9/10/19.
//

#ifndef UNTITLED2_CAMERA_H
#define UNTITLED2_CAMERA_H
#include "MathLib.h"

Vector4D random_in_unit_disk()
{
    Vector4D p;
    do {
        p = Vector4D(drand48(),drand48(),drand48())*2.0 - Vector4D(1,1,0);
    } while (p.Dot(p) >= 1.0);
    return p;
}

class camera {
public:
    camera(Vector4D lookfrom, Vector4D lookat, Vector4D vup, float vfov, float aspect, float aperture, float focus_dist)
    {
        lens_radius = aperture / 2;
        float theta = vfov*M_PI/180;
        float half_hight = tan(theta/2);
        float half_width = aspect * half_hight;
        origin = lookfrom;
        w = Vector4D::unit_vector(lookfrom - lookat);
        u = Vector4D::unit_vector(Vector4D::cross(vup, w));
        v = Vector4D::cross(w,u);
        lower_left_corner = Vector4D(-half_width, -half_hight, -1.0);
        lower_left_corner = origin - u*focus_dist*half_width - v*focus_dist*half_hight - w*focus_dist;
        horizontal = u*half_width*focus_dist*2;
        vertical = v*half_hight*focus_dist*2;
    }
    ray get_ray(float s,float t){
        Vector4D rd = random_in_unit_disk()*lens_radius;
        Vector4D offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset);
    }

    inline void moveCameraF() { origin.addTo(cameraFront * cameraSpeed); }
    inline void moveCameraB() { origin.subFrom(cameraFront * cameraSpeed); }
    inline void moveCameraL() { origin.subFrom((Vector4D::cross(cameraFront, cameraUp).Normalize() * cameraSpeed)); }
    inline void moveCameraR() { origin.addTo((Vector4D::cross(cameraFront, cameraUp).Normalize() * cameraSpeed)); }
    inline void moveCameraU() { origin.addTo(cameraUp * cameraSpeed); }
    inline void moveCameraD() { origin.subFrom(cameraUp * cameraSpeed); }
    inline void changeCameraFront(Vector4D vec) { cameraFront = vec; }

    Vector4D cameraFront = Vector4D(0,0,-1);
    Vector4D cameraUp = Vector4D(0,1,0);

    Vector4D origin;
    Vector4D lower_left_corner;
    Vector4D horizontal;
    Vector4D vertical;
    Vector4D u, v, w;
    float lens_radius;
    float cameraSpeed = 0.5f;
};
#endif //UNTITLED2_CAMERA_H

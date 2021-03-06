//
//  Framework for a raytracer
//  File: sphere.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    double B, C, d, t, theta, phi, u, v;
    Vector dir, T;
    Point hit;

    dir  = ray.D.normalized();
    T = ray.O - position;

    B = 2 * dir.dot(T);
    C = T.dot(T) - (r*r);
    d = B*B - 4*C;

    if (d < 0.0)
        return Hit::NO_HIT();
    t = (-B - sqrt(d)) / 2.0;
    if (t < 0.0)
        t = (-B + sqrt(d)) / 2.0;
    if (t < 0.0)
        return Hit::NO_HIT();

    hit = ray.at(t);
    Vector N = hit - position;
    N.normalize();

    // Calculate the color if there is a texture
    if (material->texture) {
        theta = acos((hit.z - position.z) / r);
        phi = atan2((hit.y - position.y), (hit.x - position.x));
        if (phi < 0)
            phi += 2 * M_PI;
        u = phi / (2 * M_PI);
        v = (M_PI - theta) / M_PI;
        material->color = material->texture->colorAt(u, v);
    }

    return Hit(t, N, material);
}

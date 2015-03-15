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
    double B, C, d, t;
    Vector dir, T;

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

    Vector N = ray.at(t) - position;
    N.normalize();

    return Hit(t, N, material);
}

Color Sphere::surfaceColor(const Point hit)
{
    if (!material->texture)
        return material->color;
    float theta, phi, u, v;
    theta = acos((hit.z - position.z) / r);
    phi = atan2((hit.y - position.y), (hit.x - position.x));
    if (phi < 0)
        phi += 2 * M_PI;
    u = phi / (2 * M_PI);
    v = (M_PI - theta) / M_PI;
    return material->texture->colorAt(u, v);
}

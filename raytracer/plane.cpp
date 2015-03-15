#include "plane.h"
#include <iostream>
#include <math.h>

Hit Plane::intersect(const Ray &ray)
{
    Vector dir = ray.D.normalized();
    double t, den;

    den = normal.dot(dir);
    if (den == 0)
        return Hit::NO_HIT();
    t = normal.dot(position - ray.O) / den;
    if (t < 0)
        return Hit::NO_HIT();

    return Hit(t, normal.normalized(), material);
}


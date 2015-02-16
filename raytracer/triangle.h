#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"

class Triangle: public Object
{
public:
    Triangle(Point pointA, Point pointB, Point pointC) : pointA(pointA), pointB(pointB), pointC(pointC) { }

    virtual Hit intersect(const Ray &ray);

    const Point pointA, pointB, pointC;
};

#endif /* end of include guard: SPHERE_H_115209AE */

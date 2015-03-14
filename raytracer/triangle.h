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

class SmoothTriangle : public Object
{
public:
	SmoothTriangle(Point pointA, Point pointB, Point pointC,
			Vector normalA, Vector normalB, Vector normalC) :
		Pa(pointA), Pb(pointB), Pc(pointC), Na(normalA), Nb(normalB), Nc(normalC) { }

	virtual Hit intersect(const Ray &ray);
	const Point Pa, Pb, Pc;
	const Vector Na, Nb, Nc;
};

#endif /* end of include guard: SPHERE_H_115209AE */

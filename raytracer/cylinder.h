#ifndef CYLINDER_H
#define CYLINDER_H

#include "object.h"

class Cylinder: public Object
{
public:
	Cylinder(Point position, double radius, double height):
        pos(position), r(radius), h(height) { }

	virtual Hit intersect(const Ray &ray);

    const Point pos;
	const double r, h;
private:
	Hit intersectTopBottom(const Ray &ray);
};

#endif // CYLINDER_H

#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle**********************************/

Hit Triangle::intersect(const Ray &ray)
{

	Vector u,v,n;
	double d,t;
	Point i;

	u = pointA - pointB;                                  // Defining triangle as two vectors
	v = pointC - pointB;
	n = u.cross(v).normalized();                          // Taking a cross product of those to calculate normal vector

	d = n.dot(ray.D.normalized());
	if (d == 0)
        	return Hit::NO_HIT();                         // No intersection if ray is perendicular to triangle normal

	t = n.dot(pointA - ray.O) / d;                        // No intersection if ray originates beyond the surface
	if (t < 0)
        	return Hit::NO_HIT();

	i = ray.at(t);                                        // Intersection point calculation

	if ((i-pointA).cross(pointB-pointA).dot(n) <= 0)      // No intersection if point is on the outside of the triangle edges
        	return Hit::NO_HIT();

	if ((i-pointB).cross(pointC-pointB).dot(n) <= 0)
        	return Hit::NO_HIT();

	if ((i-pointC).cross(pointA-pointC).dot(n) <= 0)
        	return Hit::NO_HIT();

	return Hit(t,n);
}

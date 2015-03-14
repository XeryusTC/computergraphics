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

Hit SmoothTriangle::intersect(const Ray &ray)
{
	Vector u, v, n;
	double d, t;
	Point i, N;

	u = Pa - Pb;
	v = Pc - Pb;
	n = u.cross(v).normalized();

	// No intersection if the ray is perpendicular to the triangle normal
	d = n.dot(ray.D);
	if (d == 0)
		return Hit::NO_HIT();

	t = n.dot(Pa - ray.O) / d;
	// No intersection if the ray originates behind the surface
	if (t < 0)
		return Hit::NO_HIT();

	i = ray.at(t);

	// No intersection if point is on the outside of the triangle edges
	if ((i - Pa).cross(Pb - Pa).dot(n) <= 0 ||
		(i - Pb).cross(Pc - Pb).dot(n) <= 0 ||
		(i - Pc).cross(Pa - Pc).dot(n) <= 0)
		return Hit::NO_HIT();

	// Interpolate normals
	Vector distA, distB, distC;
    float totArea, area1, area2, area3;
    distA = Pa - i;
    distB = Pb - i;
    distC = Pc - i;
    totArea = (Pa-Pb).cross(Pa-Pc).length();
    area1 = distB.cross(distC).length() / totArea;
    area2 = distC.cross(distA).length() / totArea;
    area3 = distB.cross(distA).length() / totArea;
    N = Na * area1 + Nb * area2 + Nc * area3;
    return Hit(t, N.normalized());
}

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

#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle**********************************/

Hit Triangle::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/Triangle intersection here.
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the Triangle, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    
//intersect if         e+td = a + beta(b-a) + gamma(c-a)

// with:               e = origin of ray                             unknown: beta, gamma, t
//                     d = direction of ray
//                     a,b,c = points defining triangle

Vector u,v,n;

u = pointA - pointB;
v = pointC - pointB;
n = u.cross(v).normalized();              // cross product

//if (n == (Vector)0)             // triangle is degenerate
//        return Hit::NO_HIT();

double d = n.dot(ray.D.normalized());
if (d == 0)
        return Hit::NO_HIT();
double t = n.dot(pointA - ray.O) / d;
if (t < 0)
        return Hit::NO_HIT();

Point i = ray.at(t);


if ((i-pointA).cross(pointB-pointA).dot(n) <= 0)
        return Hit::NO_HIT();

if ((i-pointB).cross(pointC-pointB).dot(n) <= 0)
        return Hit::NO_HIT();

if ((i-pointC).cross(pointA-pointC).dot(n) <= 0)
        return Hit::NO_HIT();

//if ((pointC-pointB).cross(i-pointB).dot(n) <= 0)
//        return Hit::NO_HIT();

//if ((pointA-pointC).cross(i-pointC).dot(n) <= 0)
//        return Hit::NO_HIT();

cerr << i << endl;
return Hit(t,n);
}

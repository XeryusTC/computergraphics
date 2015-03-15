//
//  Framework for a raytracer
//  File: hit.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//    Matthew van der Zwan
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#ifndef HIT_H_
#define HIT_H_

#include <limits>
#include "material.h"
#include "triple.h"

class Hit
{
public:
    double t;
    Vector N;
    Material *m;

    Hit(const double t, const Vector &normal, Material *material)
        : t(t), N(normal), m(material)
    { }

    static const Hit NO_HIT() { static Hit no_hit(std::numeric_limits<double>::quiet_NaN(),Vector(std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN()), NULL); return no_hit; }
};

#endif /* end of include guard: HIT_H_ */

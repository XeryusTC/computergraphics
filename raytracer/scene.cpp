//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"

Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    switch (mode) {
    case PHONG:
        return renderPhong(material, hit, N, V, ray, min_hit.t);
    case ZBUFFER:
        return renderZBuffer(hit);
    case NORMAL:
        return renderNormal(N);
    }
    return Color(0.0, 0.0, 0.0);
}

Color Scene::renderPhong(Material *m, Point hit, Vector N, Vector V, Ray orgray, double t)
{
    Color color = Color(0.0f);
    Vector L, R;
	Point i;
	bool hashit;
    for (std::vector<Light*>::iterator it=lights.begin(); it!=lights.end(); ++it) {
		hashit = false;
        // Ambient
        color += (*it)->color * m->color * m->ka;

		// Detect shadows
		if (shadows) {
			i = orgray.at(t - shadow_offset);
			Ray shadowray(i, ((*it)->position - i).normalized());
			for (std::vector<Object*>::iterator jt=objects.begin(); jt!=objects.end(); ++jt) {
				if ((*jt)->intersect(shadowray).t <= std::numeric_limits<double>::infinity()) {
					hashit = true;
					break;
				}
			}
		}
		if (hashit)
			continue;

        // Diffusion
        L = (*it)->position - hit;
        L.normalize();
        if (N.dot(L) < 0.0)
            continue;
        color += N.dot(L) * (*it)->color * m->color * m->kd;
        // Specular
        R = 2 * N.dot(L) * N - L;
        if (R.dot(V) < 0) continue;
        R.normalize();
        color += pow(V.dot(R), m->n) * (*it)->color * m->ks;
    }

    return color;
}

Color Scene::renderZBuffer(Point hit)
{
    return Color(hit.z, hit.z, hit.z);
}

Color Scene::renderNormal(Vector N)
{
    Vector C = N.normalized();
    C = (C + 1) * .5;
    return Color(C.x, C.y, C.z);
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);
            img(x,y) = col;
        }
    }

    if (mode == ZBUFFER) {
        double min=pow(2,32), max=0, scale, c;
        for (int y=0; y<h; ++y) {
            for (int x=0; x<w; ++x) {
                min = (img(x,y).r < min) ? img(x,y).r : min;
                max = (img(x,y).r > max) ? img(x,y).r : max;
            }
        }
        scale = 1 / (max-min);
        for (int y=0; y<h; ++y) {
            for (int x=0; x<w; ++x) {
                c = (img(x,y).r - min) * scale;
                img(x,y) = Color(c, c, c);
            }
        }
    }

    // Make sure the colors are not out of range
    Color col;
    for (int y=0; y<h; ++y) {
        for (int x=0; x<w; ++x) {
            col = img(x,y);
            col.clamp();
            img(x,y) = col;
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setMode(RENDER_MODE m)
{
    mode = m;
}

void Scene::setShadows(bool s)
{
	shadows = s;
}


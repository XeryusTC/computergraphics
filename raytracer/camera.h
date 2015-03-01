#ifndef CAMERA_H
#define CAMERA_H

#include "image.h"
#include "triple.h"

class Camera
{
public:
	Camera(Point p, Point c, Vector u, int w, int h):
	position(p), centre(c), up(u), width(w), height(h) { }

	Image getImage();

	Point position, centre;
	Vector up;
	int width, height;
};

#endif // CAMERA_H

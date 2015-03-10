#if defined(__APPLE__)&& defined(__MACH__)
	#include <GLUT/glut.h>
#elif defined(_WIN32)
	#include <GL/freeglut.h>
#else
	#include <GL/glut.h>
#endif

#include <math.h>
#include "camera.h"

const float TORAD = 3.141592654/180.0;

void cameraLookAt(Camera cam)
{
	float x, y, z;
        glLoadIdentity();
	cameraLookDirVector(cam, &x, &y, &z);
	gluLookAt(cam.x, cam.y, cam.z,
	          cam.x+x, cam.y+y, cam.z+z,
			  0.0, 1.0, 0.0);
};


void cameraLookDirVector(Camera cam, float *x, float *y, float *z) {
	*x = sin(cam.yaw * TORAD);
	*y = sin(cam.pitch * TORAD);
	*z = sin((cam.yaw + 90) * TORAD);
}


void cameraMoveLeft(Camera *cam, float distance)
{
	cam->x += distance * cos(cam->yaw * TORAD);
	cam->z -= distance * sin(cam->yaw * TORAD);
}

void cameraMoveRight(Camera *cam, float distance)
{
	cam->x -= distance * cos(cam->yaw * TORAD);
	cam->z += distance * sin(cam->yaw * TORAD);
}

void cameraMoveForward(Camera *cam, float distance)
{
	cam->x += distance * sin(cam->yaw * TORAD) * cos(cam->pitch * TORAD);
	cam->y += distance * sin(cam->yaw * TORAD);
	cam->z += distance * cos(cam->yaw * TORAD) * cos(cam->pitch * TORAD);
}

void cameraMoveBackward(Camera *cam, float distance)
{
	cam->x -= distance * sin(cam->yaw * TORAD) * cos(cam->pitch * TORAD);
	cam->y -= distance * sin(cam->yaw * TORAD);
	cam->z -= distance * cos(cam->yaw * TORAD) * cos(cam->pitch * TORAD);
}

void cameraMoveUp(Camera *cam, float distance)
{
	cam->y += distance;
}

void cameraMoveDown(Camera *cam, float distance)
{
	cam->y -= distance;
}
